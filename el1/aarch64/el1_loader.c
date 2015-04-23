/*
 * Copyright (c) 2015 Linaro Limited
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "el1_common.h"
#include "vmsa.h"
#include "elf.h"
#include "mem_util.h"

/* Simple ELF loader for loading EL0 image */
bool el1_load_el0(uintptr_t elfbase, uintptr_t *entry)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elfbase;
    size_t off;
    int i;

    /* Map the ELF header in so we can determine how much more to map */
    mem_map_pa(elfbase, elfbase, 0, PTE_USER_RW);

    /* Make sure this is an appropriate ELF image */
    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3) {
        DEBUG_MSG("Invalid ELF header, exiting...\n");
        SMC_EXIT();
    } else if (ehdr->e_machine != EM_ARM && ehdr->e_machine != EM_AARCH64) {
        DEBUG_MSG("Incorrect ELF type (type = %d, machine = %d), exiting...\n",
                  ehdr->e_type, ehdr->e_machine);
        SMC_EXIT();
    } else {
        DEBUG_MSG("Loading %s EL0 test image...\n",
                  (ehdr->e_machine == EM_ARM) ?  "aarch32" : "aarch64");
    }

    /* Size of the ELF to map */
    size_t elf_len = ehdr->e_shoff + (ehdr->e_shentsize * ehdr->e_shnum);

    /* Finish mapping the remainder of the ELF pages in if any */
    for (off = 0x1000; off < elf_len; off += 0x1000) {
        mem_map_pa(elfbase + off, elfbase + off, 0, PTE_USER_RW);
    }

    Elf64_Shdr *shdr = (Elf64_Shdr *)(elfbase + ehdr->e_shoff);

    Elf64_Shdr *strshdr = &shdr[ehdr->e_shstrndx];
    char *strsec = (char *)ehdr + strshdr->sh_offset;
    for (i = 0; i < ehdr->e_shnum; i++) {
        char *secname = strsec + shdr[i].sh_name;
        if (!strcmp(secname, ".text") || !strcmp(secname, ".data")) {
            uintptr_t sect = elfbase + shdr[i].sh_offset;
            uintptr_t base_va = shdr[i].sh_addr;
            DEBUG_MSG("\tloading %s section: 0x%x bytes @ 0x%lx\n",
                      secname, shdr[i].sh_size, base_va);
            for (off = 0; off < shdr[i].sh_size; off += 0x1000) {
                mem_map_va(base_va + off, 0, PTE_USER_RW);
                memcpy((void *)(base_va + off), (void *)(sect + off), 0x1000);
            }
        }
    }

    /* Unmap the FLASH ELF image */
    for (off = 0; off < elf_len; off += 0x1000) {
        mem_unmap_va(elfbase + off);
    }

    *entry = ehdr->e_entry;
    return false;
}

