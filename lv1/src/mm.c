#include <stdint.h>
#include <lv1/lv1.h>
#include <lv1/mm.h>


#define HPTE_V_BOLTED			0x0000000000000010ULL
#define HPTE_V_LARGE			0x0000000000000004ULL
#define HPTE_V_VALID			0x0000000000000001ULL
#define HPTE_R_PROT_MASK		0x0000000000000003ULL

int mm_init(void)
{
	return 0;
}

int mm_insert_htab_entry(u64 va_addr, u64 lpar_addr, u64 prot)
{
	u64 hpte_group, hpte_index, hpte_v, hpte_r, hpte_evicted_v, hpte_evicted_r;

	hpte_group = (((va_addr >> 28) ^ ((va_addr & 0xFFFFFFFULL) >> 12)) & 0x7FF) << 3;
	hpte_v = ((va_addr >> 23) << 7) | HPTE_V_VALID;
	hpte_r = lpar_addr | 0x38 | (prot & HPTE_R_PROT_MASK);

	return lv1_insert_htab_entry(0, hpte_group, hpte_v, hpte_r, HPTE_V_BOLTED, 0,
		&hpte_index, &hpte_evicted_v, &hpte_evicted_r);
}

int mm_map_lpar_memory_region(u64 lpar_start_addr, u64 ea_start_addr, u64 size,
	u64 page_shift, u64 prot)
{
	int i, result;

	for (i = 0; i < size >> page_shift; i++)
	{
		result = mm_insert_htab_entry(MM_EA2VA(ea_start_addr), lpar_start_addr, prot);
 		if (result != 0)
 			return result;

		lpar_start_addr += (1 << page_shift);
		ea_start_addr += (1 << page_shift);
	}

	return 0;
}
