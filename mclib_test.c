#include "mclib.h"

// Testing Stuff ====================
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

// List Tests ========================

typedef struct {
    int x;
    int y;
    const char* name;
} Point;
def_list(Point);
#define point(_x, _y, _name) (Point){.x = (_x), .y = (_y), .name = (_name)}

void test_list0(list(Test)* tests){
    list(Point) points = lcreate(Point, 3);
    lpush(points, point(1, 2, "A")); 
    lpush(points, point(-4, 5, "B")); 
    
    lpush(*tests, test("list0", points.count == 2));
}

void test_lists(list(Test)* tests){
    test_list0(tests);
}


int main(){
    list(Test) tests = lcreate(Test, 10); 
    printf("Running Tests...\n\n");
    
    test_lists(&tests);

    printf("Tests Complete.\n");
    print_test_report(tests);
    return 0;
}
