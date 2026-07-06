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

// Simple push test
void test_lpush0(list(Test)* tests){
    list(Point) points = lcreate(Point, 3);
    lpush(points, point(1, 2, "A")); 
    lpush(points, point(-4, 5, "B")); 
    
    lpush(*tests, test("lpush0", points.count == 2));
}

// push test with list expansion
void test_lpush1(list(Test)* tests){
    list(Point) points = lcreate(Point, 3);
    lpush(points, point(1, 2, "A")); 
    lpush(points, point(-4, 5, "B")); 
    lpush(points, point(-4, 5, "C")); 
    lpush(points, point(-4, 5, "D")); 
    
    lpush(*tests, test("lpush1", points.count == 4));
}

// simple push vector test 
void test_lpushv0(list(Test)* tests){
    list(Point) points = lcreate(Point, 3);
    lpushv(points, point(1, 2, "A"), point(4, -7, "B")); 
    
    lpush(*tests, test("lpushv0", points.count == 2));
}

// push vector test with list expansion
void test_lpushv1(list(Test)* tests){
    list(Point) points = lcreate(Point, 3);
    lpushv(points, 
            point(1, 2, "A"),
            point(4, -7, "B"),
            point(3, -4, "C"),
            point(-11, 5, "D")
          ); 
    
    lpush(*tests, test("lpushv1", points.count == 4));
}

void test_lists(list(Test)* tests){
    test_lpush0(tests);
    test_lpush1(tests);

    test_lpushv0(tests);
    test_lpushv1(tests);
}


int main(){
    list(Test) tests = lcreate(Test, 10); 
    printf("Running Tests...\n\n");
    
    test_lists(&tests);

    printf("Tests Complete.\n");
    print_test_report(tests);
    return 0;
}
