#include <stdio.h>
#include <Block.h>

typedef int (^Block)();

Block make_block()
{
    __block int counter = 1;
    return Block_copy(^ {
            counter++;
            return counter;
        });
}

int main()
{
    Block block = make_block();
    printf("Block: %d\n", block());
    printf("Block: %d\n", block());
    printf("Block: %d\n", block());
    return block();
}
