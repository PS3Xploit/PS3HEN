#define MAX_POKES 100

struct pokes {
	uint64_t addrs_backup;
	uint64_t value_backup;
	uint64_t value_pokes;
} POKES;

struct pokes *pokess=NULL;


static unsigned int poke_count = 0;

void pokeq(uint64_t address, uint64_t value)
{
	*(uint64_t *)address=value;
}

static void do_pokes(void)
{
	if(poke_count == 0) return;
	
	for(unsigned int i = 0; i < poke_count; i++)
	{
		pokeq( pokess[i].addrs_backup, pokess[i].value_pokes );
		clear_icache((void *)pokess[i].addrs_backup, 4);
	}
}

static void remove_pokes(void)
{
	if(poke_count == 0) return;

	for(unsigned int i = 0; i < poke_count; i++)
	{
		pokeq( pokess[i].addrs_backup, pokess[i].value_backup );
		clear_icache((void *)pokess[i].addrs_backup, 4);
	}
//	poke_count = 0;
}
