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
		self.grub_install=None
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
			elif arg=="--grub-install":
				index+=1
				if index>=len(args):
					print("--grub-install requires argument")
					exit(1)
				self.grub_install=args[index]
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
		print("  --grub-install <grub-install>  Specify grub-install for installing bootloader on disk image")
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
		self.determine_grub_install(args)
		print("grub-install on this system is "+self.grub_install)

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

	def determine_grub_install(self,args):
		if args.grub_install:
			self.grub_install=args.grub_install
		else:
			self.grub_install="grub-install"

	def try_load_config(self):
		file=None
		try:
			file=open("build_config.json","r")
		except FileNotFoundError:
			return False
		json_config=json.load(file)
		file.close()
		REQUIRED_VALUES=["cc","asm","ld","grub_install","cflags","asmflags","csources","ssources"]
		for val in REQUIRED_VALUES:
			if not val in json_config:
				return False
		self.cc=json_config["cc"]
		self.asm=json_config["asm"]
		self.ld=json_config["ld"]
		self.grub_install=json_config["grub_install"]
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
			"grub_install": self.grub_install,
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

def mount_system_image(format, mountpoint_folder):
	print("Following operations will mount (and format disk image), these operations will require root access.")
	print("Please check build.py source code to verify what code will be executed.")
	if format:
		check_result_code(subprocess.run(["sudo", "parted", "-s", "build/disk.img", "mklabel", "msdos", "mkpart", "primary", "fat32", "1MiB", "100%", "-a", "minimal", "set", "1", "boot", "on"]))
	check_result_code(subprocess.run(["sudo", "kpartx", "-a", "build/disk.img"]))
	result=subprocess.run(["sudo", "kpartx", "-l", "build/disk.img"], stdout=subprocess.PIPE)
	decoded_output=result.stdout.decode("UTF-8")
	parsed=decoded_output.split(" ")
	if len(parsed)<4:
		print("** Invalid output from kpartx ("+decoded_output+"). Stopping.")
		exit(1)
	device_path=parsed[4].split("/")
	if len(device_path)!=3:
		print("** Invalid device path outputted from kpartx ("+parsed[4]+"). Stopping.")
		exit(1)
	device=device_path[2]
	if format:
		check_result_code(subprocess.run(["sudo", "mkfs.exfat", "/dev/mapper/"+device+"p1"]))
	check_result_code(subprocess.run(["sudo", "mount", "/dev/mapper/"+device+"p1", mountpoint_folder, "-o", "umask=000"]))
	if format:
		check_result_code(subprocess.run(["sudo", config.grub_install, "--no-floppy", "--grub-mkdevicemap=config/grub_devicemap.map", "--modules=biosdisk part_msdos  exfat configfile normal multiboot", "--root-directory="+mountpoint_folder, "--boot-directory="+mountpoint_folder+"/boot", parsed[4], "--target=i386-pc"]))

def unmount_system_image(mountpoint_folder):
	check_result_code(subprocess.run(["sudo", "umount", mountpoint_folder]))
	check_result_code(subprocess.run(["sudo", "kpartx", "-d", "build/disk.img"]))

args=Arguments(sys.argv)
config=BuildConfig(args)

config.collect_sources()

print("** Building...")
i=1
for c in config.csources:
	if not config.should_rebuild and c in config.old_csources and config.csources[c]==config.old_csources[c] and os.path.exists("build/"+c+".o"):
		continue
	folder_path = "build/"+os.path.dirname(c)
	if not os.path.exists(folder_path):
		os.makedirs(folder_path)
	print("[" + str(i) + "/" + str(len(config.csources)) + "] " + "Compiling: [" + c + "]")
	result = subprocess.run([config.cc, "-c", c, "-o", "build/" + c + ".o"] + config.cflags)
	check_result_code(result)
	i+=1

i=1
for s in config.ssources:
	if not config.should_rebuild and s in config.old_ssources and config.old_ssources[s]==config.ssources[s] and os.path.exists("build/"+s+".o"):
		continue
	folder_path = "build/"+os.path.dirname(s)
	if not os.path.exists(folder_path):
		os.makedirs(folder_path)
	print("[" + str(i) + "/" + str(len(config.ssources)) + "] "+ "Assembling: [" + s + "]")
	result = subprocess.run([config.asm, s, "-o", "build/" + s + ".o"] + config.asmflags)
	check_result_code(result)
	i+=1

mountpoint_folder="build/mountpoint"
if not os.path.exists(mountpoint_folder):
	os.makedirs(mountpoint_folder)

total = []
for c in dict(config.csources,**config.ssources):
	total.append("build/" + c +".o")
print("** Linking kernel...")
check_result_code(subprocess.run([config.ld, "-m", "elf_i386", "-Tconfig/linker.ld", "-o", "build/kernel.elf"] + total))

if os.path.exists("build/disk.img"):
	print("** Mounting system image...")
	mount_system_image(False, mountpoint_folder)
else:
	print("** Creating new system image...")
	check_result_code(subprocess.run(["dd", "if=/dev/zero", "of=build/disk.img", "bs=1048576", "count=128"]))
	mount_system_image(True, mountpoint_folder)
check_result_code(subprocess.run(["cp", "-f", "config/grub.cfg", mountpoint_folder+"/boot/grub2"]))
check_result_code(subprocess.run(["cp", "-f", "build/kernel.elf", mountpoint_folder+"/boot/kernel.elf"]))
print("** Unmounting system image...")
unmount_system_image(mountpoint_folder)

if args.run:
	print("** Running kernel...")
	subprocess.run(["qemu-system-x86_64", "-drive", "if=ide,index=0,media=disk,format=raw,file=build/disk.img", "-m", "3G", "-vga", "vmware", "-serial", "stdio"])
