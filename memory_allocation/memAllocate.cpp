#include <iostream>
#include <vector>


class MyAllocator{

private:
    struct Block {
        size_t size;
        bool freeMemory;
        Block* next;
    };

    std::vector<char> memoryPool; //memory heap
    Block* freeList;

public:
    MyAllocator(size_t totalSize) : memoryPool(totalSize) {
    if (memoryPool.empty()) {
        std::cerr << "Error: Memory pool not allocated correctly!\n";
        exit(1);
    }
    
        freeList = reinterpret_cast<Block*>(memoryPool.data()); // linked list of memory blocks
        freeList->size = totalSize - sizeof(Block);
        freeList->freeMemory = true;
        freeList->next = nullptr;
    }

    void* allocate(size_t size){
        Block* current = freeList;
        Block* prev = nullptr;

        while(current) {
            if(current-> freeMemory && current->size >=size){
                // a mem space include header (struct Block) and free memory
                size_t remainSize = current->size - size - sizeof(Block);

                if(remainSize > sizeof(Block)){
                    
                    Block* newBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(current) + sizeof(Block) + size); // move current pointer
                    
    
                    
                    newBlock->size = remainSize;
                    newBlock->freeMemory = true;
                    newBlock->next = current->next;

                    current->size = size;
                    current->next = newBlock;

                }
             current->freeMemory = false;
            return reinterpret_cast<void*>(reinterpret_cast<char*>(current) + sizeof(Block));
       
            }         
            prev = current;
            current = current->next;

        }
        return nullptr;
    }

    void* free(void* ptr) {
        if(!ptr){
            std::cout << "Invalid pointer \n";
       
            
        }

        Block* block = reinterpret_cast<Block*>(reinterpret_cast<char*>(ptr) - sizeof(Block));
        block->freeMemory = true;

        Block* current = freeList;

        while(current && current->next){
            if(current->freeMemory && current->next->freeMemory){
                current->size += current->next->size + sizeof(Block);
                current->next = current->next->next;
            }else{
                current = current->next;
            }
        }
        return nullptr;
    }

    void show(){
        Block* current = freeList;
        std::cout << "Memory Stats: \n";
        
        while(current){
            std::cout <<"[Block Size: " << current->size << ", Free: "<< (current-> freeMemory? "Yes" : "No") << "| ->";
            current = current ->next;
        }
        std::cout <<"END_OF_LIST\n";
    }
};

int main(){
    
    MyAllocator allocator(1024);
    std::cout<< "Allocate 1024 kb in total \n";
    void* process1 = allocator.allocate(100);
    void* process2 = allocator.allocate(200);
    void* process3 = allocator.allocate(50);

    allocator.show();
    std::cout << "Free process 2 memory\n";
    allocator.free(process2);
    allocator.free(process2);
    allocator.show();
    
    std::cout<< "Allozate 1 more process\n";
    void* process4 = allocator.allocate(10);
    allocator.show();


    return 0;
}