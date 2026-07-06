#include "mclib.h"

typedef struct {
    const char* testname;
    bool pass;
} Test;
def_list(Test);
#define test(_testname, _pass) (Test){.testname = (_testname), .pass = (_pass)}


void print_test_report(list(Test) tests){
    foreach(test, tests, t_iter,
        if(test.pass){
            printf(GREEN"%s passed"NO_COLOR"\n", test.testname);
        }
        else{
            printf(RED"%s failed"NO_COLOR"\n", test.testname);
        }
    );
}

void test_lpush(list(Test)* tests){
    lpush(*tests, test("lpush", true));
}

int main(){
    list(Test) tests = lcreate(Test, 10); 
    printf("Running Tests...\n\n");
    
    test_lpush(&tests);

    printf("Tests Complete.\n");
    print_test_report(tests);
    return 0;
}
