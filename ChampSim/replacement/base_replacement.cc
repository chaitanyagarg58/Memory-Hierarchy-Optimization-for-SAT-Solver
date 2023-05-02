#include "cache.h"

uint32_t CACHE::find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    // baseline LRU replacement policy for other caches
    return lru_victim(cpu, instr_id, set, current_set, ip, full_addr, type);
}

void CACHE::update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    if (type == WRITEBACK)
    {
        if (hit) // wrietback hit does not update LRU state
            return;
    }

    return lru_update(set, way);
}

uint32_t CACHE::lfu_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    uint32_t way = 0;

    // fill invalid line first
    for (way = 0; way < NUM_WAY; way++)
    {
        if (block[set][way].valid == false)
        {

            DP(if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
        }
    }

    uint32_t temp_index = NUM_WAY;
    long long temp_lfu = LLONG_MAX;
    // LFU victim
    if (way == NUM_WAY)
    {

        for (way = 0; way < NUM_WAY; way++)
        {
            if (block[set][way].lfu < temp_lfu)
            {
                temp_lfu = block[set][way].lfu;
                temp_index = way;
            }
        }
        // if (block[set][way].lfu == NUM_WAY-1) {
        way = temp_index;

        DP(if (warmup_complete[cpu]) {
        cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
        cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
        cout << dec << " lru: " << block[set][way].lru << endl; });

        // break;
        // }
    }

    // if (way == NUM_WAY) {
    //     cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
    //     assert(0);
    // }

    return way;
}

uint32_t CACHE::random_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    uint32_t way = 0;

    // fill invalid line first
    for (way = 0; way < NUM_WAY; way++)
    {
        if (block[set][way].valid == false)
        {

            DP(if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
        }
    }

    // RANDOM Victim
    if (way == NUM_WAY)
    {
        way = rand() % NUM_WAY;

        DP(if (warmup_complete[cpu]) {
        cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
        cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
        cout << dec << " lru: " << block[set][way].lru << endl; });
    }
    // if (way == NUM_WAY) {
    //     cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
    //     assert(0);
    // }

    return way;
}

uint32_t CACHE::fifo_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    uint32_t way = 0;

    // fill invalid line first
    for (way = 0; way < NUM_WAY; way++)
    {
        if (block[set][way].valid == false)
        {

            DP(if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
        }
    }

    // FIFO victim
    if (way == NUM_WAY)
    {
        for (way = 0; way < NUM_WAY; way++)
        {
            if (block[set][way].fifo == NUM_WAY - 1)
            {

                DP(if (warmup_complete[cpu]) {
                cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
                cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
                cout << dec << " lru: " << block[set][way].lru << endl; });

                break;
            }
        }
    }

    if (way == NUM_WAY)
    {
        cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
        assert(0);
    }

    return way;
}

uint32_t CACHE::lru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    uint32_t way = 0;

    // fill invalid line first
    for (way = 0; way < NUM_WAY; way++)
    {
        if (block[set][way].valid == false)
        {

            DP(if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
        }
    }

    // LRU victim
    if (way == NUM_WAY)
    {
        for (way = 0; way < NUM_WAY; way++)
        {
            if (block[set][way].lru == NUM_WAY - 1)
            {

                DP(if (warmup_complete[cpu]) {
                cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
                cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
                cout << dec << " lru: " << block[set][way].lru << endl; });

                break;
            }
        }
    }

    if (way == NUM_WAY)
    {
        cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
        assert(0);
    }

    return way;
}


uint32_t CACHE::prob_lfu_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    uint32_t way = 0;

    // fill invalid line first
    for (way = 0; way < NUM_WAY; way++)
    {
        if (block[set][way].valid == false)
        {

            DP(if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
        }
    }

    uint32_t temp_index = NUM_WAY;
    long long temp_lfu = LLONG_MAX;

    double probabilities[NUM_WAY];
    double sum_probabilities = 0.0;

    for (int way1 = 0; way1 < NUM_WAY; way1++)
    {
        if (block[set][way1].valid)
        {
            probabilities[way1] = exp(-block[set][way1].lfu);
            sum_probabilities += probabilities[way1];
        }
        else
        {
            probabilities[way1] = 0.0;
        }
    }

    for (int way1 = 0; way1 < NUM_WAY; way1++)
    {
        probabilities[way1] /= sum_probabilities;
    }

    // LFU victim
    if (way == NUM_WAY)
    {

        // for (way = 0; way < NUM_WAY; way++)
        // {
        //     if (block[set][way].lfu < temp_lfu)
        //     {
        //         temp_lfu = block[set][way].lfu;
        //         temp_index = way;
        //     }
        // }
        // if (block[set][way].lfu == NUM_WAY-1) {

        double random_num = (double)rand() / RAND_MAX;
        double prob_sum = 0.0;
        for (way = 0; way < NUM_WAY; way++)
        {
            prob_sum += probabilities[way];
            if (prob_sum >= random_num)
            {
                temp_index = way;
                break;
            }
        }
        way = temp_index;

        DP(if (warmup_complete[cpu]) {
        cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
        cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
        cout << dec << " lru: " << block[set][way].lru << endl; });

        // break;
        // }
    }

    // if (way == NUM_WAY) {
    //     cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
    //     assert(0);
    // }

    return way;
}


uint32_t CACHE::mru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    uint32_t way = 0;

    // fill invalid line first
    for (way = 0; way < NUM_WAY; way++)
    {
        if (block[set][way].valid == false)
        {

            DP(if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
        }
    }

    uint32_t way_index = 0;

    // MRU victim
    if (way == NUM_WAY)
    {
        for (way = 1; way < NUM_WAY; way++)
        {
            if (block[set][way].lru == 0)
            {
                way_index = way;
                break;
            }
        }
        DP(if (warmup_complete[cpu]) {
        cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
        cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
        cout << dec << " lru: " << block[set][way].lru << endl; });

        way = way_index;
    }

    if (way == NUM_WAY)
    {
        cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
        assert(0);
    }

    return way;
}

uint32_t CACHE::prob_mru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    uint32_t way = 0;

    // fill invalid line first
    for (way = 0; way < NUM_WAY; way++)
    {
        if (block[set][way].valid == false)
        {

            DP(if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
            return way;
        }
    }

    uint32_t way_index = 0;

    double probabilities[NUM_WAY];
    double sum_probabilities = 0.0;

    for (int way1 = 0; way1 < NUM_WAY; way1++)
    {
        if (block[set][way1].valid)
        {
            probabilities[way1] = NUM_WAY-block[set][way1].lru;
            sum_probabilities += probabilities[way1];
        }
        else
        {
            probabilities[way1] = 0.0;
        }
    }

    for (int way1 = 0; way1 < NUM_WAY; way1++)
    {
        probabilities[way1] /= sum_probabilities;
    }


    // MRU victim
    if (way == NUM_WAY)
    {
        // for (way = 1; way < NUM_WAY; way++)
        // {
        //     if (block[set][way].lru == 0)
        //     {
        //         way_index = way;
        //         break;
        //     }
        // }
        double random_num = (double)rand() / RAND_MAX;
        double prob_sum = 0.0;
        for (way = 0; way < NUM_WAY; way++)
        {
            prob_sum += probabilities[way];
            if (prob_sum >= random_num)
            {
                way_index = way;
                break;
            }
        }
        DP(if (warmup_complete[cpu]) {
        cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
        cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
        cout << dec << " lru: " << block[set][way].lru << endl; });

        way = way_index;
    }

    if (way == NUM_WAY)
    {
        cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
        assert(0);
    }

    return way;
}

void CACHE::fifo_update(uint32_t set, uint32_t way)
{
    for (uint32_t i = 0; i < NUM_WAY; i++)
    {
        if (block[set][i].valid)
        {
            block[set][i].fifo++;
        }
    }
    block[set][way].fifo = 0;
}

uint32_t CACHE::trash_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    return 0;
}
void CACHE::trash_update(uint32_t set, uint32_t way)
{
}

void CACHE::lru_update(uint32_t set, uint32_t way)
{
    // update lru replacement state
    for (uint32_t i = 0; i < NUM_WAY; i++)
    {
        if (block[set][i].lru < block[set][way].lru)
        {
            block[set][i].lru++;
        }
    }
    block[set][way].lru = 0; // promote to the MRU position
}

void CACHE::lfu_update(uint32_t set, uint32_t way, bool myhit)
{
    if (!myhit)
        block[set][way].lfu = 0;
    block[set][way].lfu++;
}

void CACHE::random_update(uint32_t set, uint32_t way)
{
}

void CACHE::replacement_final_stats()
{
}

#ifdef NO_CRC2_COMPILE
void InitReplacementState()
{
}

uint32_t GetVictimInSet(uint32_t cpu, uint32_t set, const BLOCK *current_set, uint64_t PC, uint64_t paddr, uint32_t type)
{
    return 0;
}

void UpdateReplacementState(uint32_t cpu, uint32_t set, uint32_t way, uint64_t paddr, uint64_t PC, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
}

void PrintStats_Heartbeat()
{
}

void PrintStats()
{
}
#endif
