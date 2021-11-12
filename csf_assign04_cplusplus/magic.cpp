#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <iostream>

#include "elf_names.h"

int handle_arguments(int argc, char* filename);
void print_summary(uint16_t objtype, uint16_t machtype, uint16_t endianness);
void print_section_summary(unsigned char* elf_header_uc, int symbol_sec[2]);
void print_symbol_summary(unsigned char* elf_header_uc, int sym_index, int sym_name_index);

int main(int argc, char **argv) {

  // First, make sure to handle all of the argyments
  int fd = handle_arguments(argc, argv[1]);
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  void *data;
  if (rc != 0) {
    fprintf(stdout, "Could not get file size\n");
    exit(3);
  } else {
    size_t file_size = statbuf.st_size;
    data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == (void *)-1) {
      fprintf(stdout, "Could not map file\n");
      exit(4);
    }
  }

  // Make sure we have an ELF file using magic numbers listed on Wikipedia
  unsigned char *elf_header_uc = static_cast<unsigned char*> (data);
  if (*elf_header_uc != 0x7F || *(elf_header_uc+1) != 'E' || *(elf_header_uc+2) != 'L' || *(elf_header_uc+3) != 'F') {
    fprintf(stdout, "Not an ELF file\n");
    exit(0);
  }

  Elf64_Ehdr *elf_header = reinterpret_cast<Elf64_Ehdr *> (elf_header_uc);
  print_summary((uint16_t)elf_header->e_type, (uint16_t)elf_header->e_machine, (uint16_t)elf_header->e_ident[EI_DATA]);
  int sym_sec[2];
  print_section_summary(elf_header_uc, sym_sec);
  int sym_index = sym_sec[0];
  int sym_name_index = sym_sec[1];
  print_symbol_summary(elf_header_uc, sym_index, sym_name_index);
  
}

// Handle the potential inputs
int handle_arguments(int argc, char* filename) {
  if (argc != 2) {
    fprintf(stderr, "Incorrect number of arguments!\n");
    exit(1);
  }

  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    fprintf(stderr, "Could not open File!\n");
    exit(2);
  }
  return fd;
}

// Print the summary with the ELF file
void print_summary(uint16_t objtype, uint16_t machtype, uint16_t endianness) {
  printf("Object file type: %s\n", get_type_name(objtype));
  printf("Instruction set: %s\n", get_machine_name(machtype));
  if (endianness == 1) {
    printf("Endianness: Little endian\n");
  } else if (endianness == 2) {
    printf("Endianness: Big endian\n");
  } else {
    fprintf(stderr, "Endianness Unknown Error\n");
    exit(5);
  }
}

// Prints all of the required section data
void print_section_summary(unsigned char* elf_header_uc, int symbol_sec[2]) {
  Elf64_Ehdr *elf_header = reinterpret_cast<Elf64_Ehdr *> (elf_header_uc);
  int sec_size = elf_header->e_shentsize;

  // Next, get a pointer to section names
  unsigned char *sec_headers_top = elf_header_uc + elf_header->e_shoff;
  unsigned char *sec_names_uc = sec_headers_top + ((elf_header->e_shstrndx) * sec_size);
  Elf64_Shdr *sec_names = reinterpret_cast<Elf64_Shdr *> (sec_names_uc);

  int num_headers = elf_header->e_shnum;
  for (int i = 0; i < num_headers; i++) {
    unsigned char *sec_header_uc = sec_headers_top + (i * sec_size);
    Elf64_Shdr *sec_header = reinterpret_cast<Elf64_Shdr *>(sec_header_uc); // pointer to section header
    char* name = (char*) (elf_header_uc + sec_names->sh_offset + sec_header->sh_name); // gets the section name
    printf("Section header %d: name=%s, ", i, name);
    printf("type=%lx, ", (long unsigned int) sec_header->sh_type);
    printf("offset=%lx, ", (long unsigned int) sec_header->sh_offset);
    printf("size=%lx\n", (long unsigned int) sec_header->sh_size);

    if (strcmp(name, ".symtab") == 0) { // Save the index of .symtab
      symbol_sec[0] = i;
    }
    if (strcmp(name, ".strtab") == 0) {  // Save the index of .strtab
      symbol_sec[1] = i;
    }
  }
}

// Prints all of the required symbol data
void print_symbol_summary(unsigned char* elf_header_uc, int sym_index, int sym_name_index) {
  // Get a pointer to section .symtab
  Elf64_Ehdr *elf_header = reinterpret_cast<Elf64_Ehdr *> (elf_header_uc);
  unsigned char *sec_headers_top = elf_header_uc + elf_header->e_shoff;
  int sec_size = elf_header->e_shentsize;

  unsigned char *sec_symbols_uc = sec_headers_top + (sym_index * sec_size);
  Elf64_Shdr *sec_symbols = reinterpret_cast<Elf64_Shdr *> (sec_symbols_uc);
  int num_sym = sec_symbols->sh_size / sec_symbols->sh_entsize;

  // Also, get a pointer to section .strtab
  unsigned char *sec_strtab_uc = sec_headers_top + (sym_name_index * sec_size);
  Elf64_Shdr *sec_strtab = reinterpret_cast<Elf64_Shdr *> (sec_strtab_uc);

  // Print output for all symbols
  for (int i = 0; i < num_sym; i++) {
    unsigned char *sym_uc = (unsigned char*)(elf_header_uc + sec_symbols->sh_offset + (i * sec_symbols->sh_entsize));
    Elf64_Sym *sym = reinterpret_cast<Elf64_Sym *>(sym_uc); // pointer to symbol headers
    char* name = (char*) (elf_header_uc + sec_strtab->sh_offset + sym->st_name); // gets the symbol names
    printf("Symbol %d: name=%s, ", i, name);
    printf("size=%lx, ", (long unsigned int) sym->st_size);
    printf("info=%lx, ", (long unsigned int) sym->st_info);
    printf("other=%lx\n", (long unsigned int) sym->st_other);
  }
}