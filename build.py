#!/usr/bin/python3

# QuantumOS Copyright (c) 2022-2023
# - SolindekDev <solindeklive.biznes@gmail.com>
# - mrkubax10 <mrkubax10@onet.pl>

import fnmatch
import os
import sys
import json
import subprocess

class Arguments:
	def __init__(self,args):
		self.cc=None
		self.asm=None
		self.ld=None
		self.grub_rescue=None
		self.run=False
		self.should_rebuild=False
		index=1
		while index<len(args):
			arg = args[index]
			if arg=="--cc":
				index+=1
				if index>=len(args):
					print("--cc requires argument")
					exit(1)
				self.cc=args[index]
				self.should_rebuild=True
			elif arg=="--asm":
				index+=1
				if index>=len(args):
					print("--asm requires argument")
					exit(1)
				self.asm=args[index]
				self.should_rebuild=True
			elif arg=="--ld":
				index+=1
				if index>=len(args):
					print("--ld requires argument")
					exit(1)
				self.ld=args[index]
			elif arg=="--grub-rescue":
				index+=1
				if index>=len(args):
					print("--grub-rescue requires argument")
					exit(1)
				self.grub_rescue=args[index]
			elif arg=="run":
				self.run=True
			elif arg=="help" or arg=="--help" or arg=="-h":
				self.print_help(args[0])
				exit(0)
			elif arg=="--rebuild":
				self.should_rebuild=True
			else:
				print("Invalid argument: "+arg)
				exit(1)
			index+=1

	def print_help(self,executable):
		print("QuantumOS build system")
		print("Usage: "+executable+" [subcommand] [arguments...]")
		print("Subcommands:")
		print("  run                            Start QEMU after build finishes")
		print("  help                           Show this message")
		print("Arguments:")
		print("  --cc <compiler>                Specify C compiler")
		print("  --asm <assembler>              Specify assembler")
		print("  --ld <linker>                  Specify linker")
		print("  --grub-rescue <grub rescue>    Specify GRUB Rescue for creating ISO image")
		print("  --rebuild                      Full rebuild")


class BuildConfig:
	def __init__(self,args):
		self.csources={}
		self.ssources={}
		self.should_rebuild=args.should_rebuild
		self.collect_sources()
		if args.should_rebuild or not self.try_load_config():
			self.generate_config(args)
		self.write_config()

	def collect_sources(self):
		for root, dirnames, filenames in os.walk('.'):
			for filename in fnmatch.filter(filenames, '*.c'):
				p = os.path.join(root, filename)
				self.csources[p]=os.path.getmtime(p)

		for root, dirnames, filenames in os.walk('.'):
			for filename in fnmatch.filter(filenames, '*.asm'):
				p = os.path.join(root, filename)
				self.ssources[p]=os.path.getmtime(p)

	def generate_config(self,args):
		self.should_rebuild=True
		print("** Generating build config...")
		self.determine_cc(args)
		print("C compiler on this system is " + self.cc)
		self.determine_asm(args)
		print("Assembler on this system is " + self.asm)
		self.determine_ld(args)
		print("Linker on this system is "+self.ld)
		self.determine_grub_rescue(args)
		print("GRUB Rescue on this system is "+self.grub_rescue)

	def determine_cc(self,args):
		if args.cc:
			self.cc=args.cc
		else:
			self.cc=os.getenv("CC","cc")

	def determine_asm(self,args):
		if args.asm:
			self.asm=args.asm
		else:
			self.asm="nasm"

	def determine_ld(self,args):
		if args.ld:
			self.ld=args.ld
		else:
			self.ld="ld"

	def determine_grub_rescue(self,args):
		if args.grub_rescue:
			self.grub_rescue=args.grub_rescue
		else:
			self.grub_rescue="grub-mkrescue"

	def try_load_config(self):
		file=None
		try:
			file=open("build_config.json","r")
		except FileNotFoundError:
			return False
		json_config=json.load(file)
		file.close()
		REQUIRED_VALUES=["cc","asm","ld","grub_rescue","cflags","asmflags","csources","ssources"]
		for val in REQUIRED_VALUES:
			if not val in json_config:
				return False
		self.cc=json_config["cc"]
		self.asm=json_config["asm"]
		self.ld=json_config["ld"]
		self.grub_rescue=json_config["grub_rescue"]
		self.cflags=json_config["cflags"]
		self.asmflags=json_config["asmflags"]
		self.old_csources=json_config["csources"]
		self.old_ssources=json_config["ssources"]
		return True

	def write_config(self):
		self.cflags=["-m32", "-Wno-int-to-pointer-cast", "-nostdlib", "-nostdinc", "-fno-builtin", "-fno-stack-protector", "-nostartfiles", "-nodefaultlibs", "-I./include/"]
		self.asmflags=["-f", "elf"]
		json_config={
			"cc": self.cc,
			"asm": self.asm,
			"ld": self.ld,
			"grub_rescue": self.grub_rescue,
			"cflags": self.cflags,
			"asmflags": self.asmflags,
			"csources": self.csources,
			"ssources": self.ssources
		}
		file=open("build_config.json","w")
		json.dump(json_config, file)
		file.close()


def check_result_code(result):
	if result.returncode != 0:
		print("** Previous subcommand failed. Stopping.")
		exit(1)


args=Arguments(sys.argv)
config=BuildConfig(args)

config.collect_sources()

print("** Building...")
i=1
for c in config.csources:
	if not config.should_rebuild and c in config.old_csources and config.csources[c]==config.old_csources[c]:
		continue
	folder_path = "build/"+os.path.dirname(c)
	if not os.path.exists(folder_path):
		os.makedirs(folder_path)
	print("[" + str(i + 1) + "/" + str(len(config.csources)) + "] " + "Compiling: [" + c + "]")
	result = subprocess.run([config.cc, "-c", c, "-o", "build/" + c + ".o"] + config.cflags)
	check_result_code(result)
	i+=1

i=1
for s in config.ssources:
	if not config.should_rebuild and s in config.old_ssources and config.old_ssources[s]==config.ssources[s]:
		continue
	folder_path = "build/"+os.path.dirname(s)
	if not os.path.exists(folder_path):
		os.makedirs(folder_path)
	print("[" + str(i + 1) + "/" + str(len(config.ssources)) + "] "+ "Assembling: [" + s + "]")
	result = subprocess.run([config.asm, s, "-o", "build/" + s + ".o"] + config.asmflags)
	check_result_code(result)
	i+=1

grub_folder="build/iso/boot/grub"
if not os.path.exists(grub_folder):
	os.makedirs(grub_folder)

total = []
for c in dict(config.csources,**config.ssources):
	total.append("build/" + c +".o")

print("** Linking kernel...")
result = subprocess.run([config.ld, "-m", "elf_i386", "-Tconfig/linker.ld", "-o", "build/iso/boot/kernel.elf"] + total)
check_result_code(result)
result = subprocess.run(["cp", "-f", "config/grub.cfg", "build/iso/boot/grub"])
check_result_code(result)
result = subprocess.run([config.grub_rescue, "-o", "build/quantumos.iso", "build/iso"])
check_result_code(result)

if args.run:
	print("** Running kernel...")
	subprocess.run(["qemu-system-x86_64", "-cdrom", "./build/quantumos.iso", "-m", "3G", "-vga", "vmware", "-serial", "stdio"])
