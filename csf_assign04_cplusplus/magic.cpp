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

using std::cerr;
using std::cout;
using std::endl;
using std::string;


int handle_arguments(int argc, char* filename);
void print_summary(uint16_t objtype, uint16_t machtype, uint16_t endianness);

/*
TODO:
1. Where do we do "not an elf file"?
2. 
*/

int main(int argc, char **argv) {

  // First, make sure to handle all of the argyments
  int fd = handle_arguments(argc, argv[1]);
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  void *data;
  if (rc != 0) {
    cerr << "Could not get file size" << endl;
    exit(3);
  } else {
    size_t file_size = statbuf.st_size;
    data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  }

  // MUST USE UNSIGNED CHAR FOR ADDRESS COMPUTATIONS
  // First, get all the info regarding the elf header
  unsigned char *elf_header_uc = static_cast<unsigned char*> (data);
  Elf64_Ehdr *elf_header = reinterpret_cast<Elf64_Ehdr *> (elf_header_uc);
  int sec_size = elf_header->e_shentsize;

  // Next, get a pointer to section names
  unsigned char *sec_headers_top = elf_header_uc + elf_header->e_shoff;
  unsigned char *sec_names_uc = sec_headers_top + ((elf_header->e_shstrndx) * sec_size);
  Elf64_Shdr *sec_names = reinterpret_cast<Elf64_Shdr *> (sec_names_uc);

  print_summary((uint16_t)elf_header->e_type, (uint16_t)elf_header->e_machine, (uint16_t)elf_header->e_ident[EI_DATA]);

  // Get all of the information about sections
  int num_headers = elf_header->e_shnum;
  int sym_index = -1;
  int sym_name_index = -1;
  for (int i = 0; i < num_headers; i++) {
    unsigned char *sec_header_uc = sec_headers_top + (i * sec_size);
    Elf64_Shdr *sec_header = reinterpret_cast<Elf64_Shdr *>(sec_header_uc); // pointer to section headers
    cout << "Section header " << i << ": ";
    char* name = (char*) (elf_header_uc + sec_names->sh_offset + sec_header->sh_name);
    printf("name=%s, ", name);
    printf("type=%lx, ", sec_header->sh_type);
    printf("offset=%lx, ", sec_header->sh_offset);
    printf("size=%lx\n", sec_header->sh_size);

    if (strcmp(name, ".symtab") == 0) {
      sym_index = i;
    }
    if (strcmp(name, ".strtab") == 0) {
      sym_name_index = i;
    }
  }

  // Get all of the information about symbols
  // Next, get a pointer to section .symtab
  unsigned char *sec_symbols_uc = sec_headers_top + (sym_index * sec_size);
  Elf64_Shdr *sec_symbols = reinterpret_cast<Elf64_Shdr *> (sec_symbols_uc);
  int num_sym = sec_symbols->sh_size / sec_symbols->sh_entsize;

  // Also, get a pointer to section .strtab
  unsigned char *sec_strtab_uc = sec_headers_top + (sym_name_index * sec_size);
  Elf64_Shdr *sec_strtab = reinterpret_cast<Elf64_Shdr *> (sec_strtab_uc);

  //cout << sec_strtab->sh_offset << endl;
  for (int i = 0; i < num_sym; i++) {
    unsigned char *sym_uc = (unsigned char*)(elf_header_uc + sec_symbols->sh_offset + (i * sec_symbols->sh_entsize));
    Elf64_Sym *sym = reinterpret_cast<Elf64_Sym *>(sym_uc); // pointer to section headers
    cout << "Symbol " << i << ": ";
    char* name = (char*) (elf_header_uc + sec_strtab->sh_offset + sym->st_name);
    printf("Symbol: %d: name=%s, size=%lx, info=%lx, other=%lx, \n", i, name, sym->st_size, sym->st_info, sym->st_other);
    
  }

  


  











  /*
  printf("offset of section headers is %lu\n", elf_header->e_shoff);
  printf("number of section headers is %u\n", elf_header->e_shnum);
  printf(".shstrtab section index is %u\n", elf_header->e_shstrndx);

  cout << "header:" << elf_header_uc << endl;
  cout << "offset:" << sec_names->sh_offset << endl;
  cout << "name offset:" << sec_names->sh_name << endl;

  cout << "shstrtab data is " << sec_names->sh_offset << endl;
  cout << "shstrtab size is " << sec_names->sh_size << endl;
  cout << "shstrtab name is " << sec_names->sh_name << endl;


  cout << (char*)(elf_header_uc + sec_names->sh_offset + sec_headers2->sh_name) << endl;
  */
  //Elf64_Shdr *section_names = (Elf64_Shdr *) (section_header+elf_header->e_shstrndx);
  //unsigned char *section_names = (unsigned char *) elf_header+elf_header->e_shstrndx;  
  //char* name = (char*) elf_header+elf_header->e_shstrndx;
  //cout << (char*) (section_names+(section_names->sh_offset)) << endl;
  //printf("section name is %s\n", (char*) section_names+section_header->sh_name);
  //printf("elf type name is %s\n", get_type_name(elf_header->e_type));
  
}

int handle_arguments(int argc, char* filename) {
  if (argc != 2) {
    cerr << "Incorrect number of arguments!" << endl;
    exit(1);
  }

  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    cerr << "Could not open File" << endl;
    exit(2);
  }
  return fd;  
}

void print_summary(uint16_t objtype, uint16_t machtype, uint16_t endianness) {
  cout << "Object file type: " << get_type_name(objtype) << endl;
  cout << "Instruction set: " << get_machine_name(machtype) << endl;
  if (endianness == 1) {
    cout << "Endianness: " << "Little endian" << endl;
  } else if (endianness == 2) {
    cout << "Endianness: " << "Big endian" << endl;
  } else {
    cerr << "Endianness Unknown Error" << endl;
    exit(4);
  }
  
}