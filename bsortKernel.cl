#define GROUP_SIZE 256
#define CHALLENGE_LEN 32

typedef struct entry_s{
    char challenge[CHALLENGE_LEN+1];
    int  idx;
}entry_t;

/*
 * compare tow entry_t object
 */
int compare(entry_t *t1, entry_t *t2, int len)
{
    char *p1 = t1->challenge;
    char *p2 = t2->challenge;
    for(int i = 0; i < len; i++)
    {
        if(p1[i] == p2[i]){
            continue;
        }else if(p1[i] < p2[i]){
            return -1;
        }else{
            return 1;
        }
    }
    return 0;
}

__kernel
void parallelBitonicSort(__global entry_t * data,
                         const uint stage,
                         const uint subStage,
                         const uint direction) {

    uint threadId = get_global_id(0);
    uint sortIncreasing = direction;

    uint distanceBetweenPairs = 1 << (stage - subStage);
    uint blockWidth   = distanceBetweenPairs << 1;

    uint leftId = (threadId % distanceBetweenPairs) + (threadId / distanceBetweenPairs) * blockWidth;

    uint rightId = leftId + distanceBetweenPairs;

    entry_t leftElement = data[leftId];
    entry_t rightElement = data[rightId];

    uint sameDirectionBlockWidth = 1 << stage;

    if((threadId/sameDirectionBlockWidth) % 2 == 1)
        sortIncreasing = 1 - sortIncreasing;

    entry_t greater;
    entry_t lesser;
    if(compare(&leftElement, &rightElement, CHALLENGE_LEN) > 0) {
        greater = leftElement;
        lesser  = rightElement;
    } else {
        greater = rightElement;
        lesser  = leftElement;
    }

    if(sortIncreasing) {
        data[leftId]  = lesser;
        data[rightId] = greater;
    } else {
        data[leftId]  = greater;
        data[rightId] = lesser;
    }
}

