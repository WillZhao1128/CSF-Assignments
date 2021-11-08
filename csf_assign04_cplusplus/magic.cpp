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
void print_summary(string objtype, string machtype, string endianness);

/*
TODO:
1. Where do we do "not an elf file"?
2. 
*/

int main(int argc, char **argv) {
  // TODO: implement
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
  unsigned char *elf_header_uc = (unsigned char *) data;
  Elf64_Ehdr *elf_header = reinterpret_cast<Elf64_Ehdr *> (elf_header_uc);
  int sec_size = elf_header->e_shentsize;

  // Next, get a pointer to section names
  unsigned char *sec_headers_top = elf_header_uc + elf_header->e_shoff;
  Elf64_Shdr *sec_headers = reinterpret_cast<Elf64_Shdr *>(sec_headers_top); // pointer to start of section headers
  unsigned char *sec_names_uc = sec_headers_top + ((elf_header->e_shstrndx) * sec_size);
  Elf64_Shdr *sec_names = reinterpret_cast<Elf64_Shdr *> (sec_names_uc);
  
  cout << elf_header->e_shnum << endl;

  int num_headers = elf_header->e_shnum;
  for (int i = 0; i < num_headers; i++) {
    unsigned char *sec_header_uc = sec_headers_top + (i * sec_size);
    Elf64_Shdr *sec_header = reinterpret_cast<Elf64_Shdr *>(sec_header_uc); // pointer to section headers
    cout << (char*)(elf_header_uc + sec_names->sh_offset + sec_header->sh_name) << endl;
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

void print_summary(string objtype, string machtype, string endianness) {
  cout << "Object file type: " << objtype << endl;
  cout << "Instruction set: " << machtype << endl;
  cout << "Endianness: " << endianness << endl;
}