#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SUCCESS 0
#define NOT_PRESENT -1
#define NOT_ACCESS -2
#define PRESENT_MASK 0b01
#define ACCESS_MASK 0b10
#define FRAME_NUM_SHIFT 12

unsigned int vpn_mask = 0;
unsigned int offset_mask = 0;

unsigned int page_size_in_bits = 0;

unsigned int *page_table = NULL;

void alloc_page_table(int address_space_bits, int page_bytes);
void init_page_table(int address_space_bits, int page_bytes);
void init_mmu_var(int address_space_bits, int page_bytes);
int address_trans(unsigned int virtual_address, unsigned int *physical_address);

void alloc_page_table(int address_space_bits, int page_bytes)
{
    int page_table_length;
    page_table_length=pow(2,address_space_bits-(int)log2(page_bytes));
    page_table=(int *)malloc(sizeof(page_table_length));
    memset(page_table,0,sizeof(page_table_length));

}

void init_page_table(int address_space_bits, int page_bytes)
{
    unsigned int i;
    int page_table_length = pow(2, address_space_bits - (int)log2(page_bytes));

    for (i = 0; i < page_table_length / 2; i++)
    {
        unsigned int *pte = page_table + i;
        *pte = (i * 2) << FRAME_NUM_SHIFT;
        if (i % 4 == 0) {
            *pte = *pte | PRESENT_MASK;
        }
        else {
            *pte = *pte | PRESENT_MASK | ACCESS_MASK;
        }
    }
}

void init_mmu_var(int address_space_bits, int page_bytes)
{
    page_size_in_bits = (int)log2(page_bytes);
    vpn_mask = (0xffffffff >> page_size_in_bits) << page_size_in_bits;
    offset_mask = 0xffffffff >> (sizeof(offset_mask) * 8 - page_size_in_bits);
}

int address_trans(unsigned int virtual_address, unsigned int *physical_address)
{
    // your code
    unsigned int vpn = virtual_address;
    unsigned int pte = &physical_address;
    unsigned int pfn = ;
    int present = ;
    int access = ;
    unsigned int offset = ;

    printf(" (vpn:%08x, pfn: %08x, valid: %d, access: %d) ", vpn, pfn, present, access);

    return SUCCESS;
}

int main(int argc, char *argv[])
{
    printf("+--------------------------------------------------+\n");
    printf("| Welcome! This is a Software implemented SSU_MMU  |\n");
    printf("| Made by Operator Sytems Lab., Soongsil University|\n");
    printf("|                 December 19 2020                 |\n");
    printf("+--------------------------------------------------+\n");


    if (argc != 3)
    {
        printf("Usage: ./mmu [address_space_size_in_bits] [page_size_in_bytes]\n");
        exit(1);
    }

    int address_space_bits = atoi(argv[1]);
    int page_bytes = atoi(argv[2]);

    if (address_space_bits < 1 || address_space_bits > 32)
    {
        printf("address_space_bits shoud be between 1 and 32\n");
        exit(1);
    }

    if (page_bytes < 1 || page_bytes > 4096)
    {
        printf("page_bytes shoud be between 1 and 4096\n");
        exit(1);
    }

    alloc_page_table(address_space_bits, page_bytes);

    if (page_table == NULL)
    {
        printf("Please allocate the page table with malloc()\n");
        exit(1);
    }

    init_page_table(address_space_bits, page_bytes);
    init_mmu_var(address_space_bits, page_bytes);

    while (1)
    {
        unsigned int virtual_address;
        printf("Input a Virtual address of Hexadecimal value without \"0x\" (-1 to exit): ");
        scanf("%x", &virtual_address);

        if (virtual_address == -1)
            break;

        printf("Virtual address: %#x", virtual_address);

        unsigned int physical_address = 0;
        int result = address_trans(virtual_address, &physical_address);
        if (result == NOT_PRESENT)
        {
            printf(" -> Segmentation fault.\n");
        }
        else if (result == NOT_ACCESS)
        {
            printf(" -> Protection fault.\n");
        }
        else if (result == SUCCESS)
        {
            printf(" -> Physical address: %#x\n", physical_address);
        }
        else
        {
            printf(" -> Unknown error.\n");
        }
    }

    if (page_table != NULL)
        free(page_table);

    return 0;
}
