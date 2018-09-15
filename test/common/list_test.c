#include <stdlib.h>
#include <string.h>

#include "list_test.h"
#include "common/list.h"

TEST(testCreateEmptyList, {
    List* l = newList();
    ASSERT_EQ_INT(0, l->count, "List should be empty");
})

typedef struct {
    char* someStr;
    char** stringArr;
} SomeObj;

SomeObj* newSomeObj(char* someStr, char** stringArr) {
    SomeObj* so = malloc(sizeof(SomeObj*));
    so->someStr = someStr;
    so->stringArr = stringArr;
    return so;
}

TEST(testStoringAndRetrievingObject, {
    List* l = newList();

    SomeObj* someObj = newSomeObj("Some String", (char* []) {"String 1", "String 2"});
    listAdd(l, (void**) &someObj);
    ASSERT_EQ_INT(1, l->count, "List should have 1 item in it");

    SomeObj* retrieved = l->values[0];
    ASSERT_EQ_STR("Some String", retrieved->someStr, "The `someStr` field of the retrieved object should be correct");
    ASSERT_EQ_STR("String 1", retrieved->stringArr[0], "The 0th element of the `stringArr` field should match");
    ASSERT_EQ_STR("String 2", retrieved->stringArr[1], "The 1st element of the `stringArr` field should match");
})

TEST(testAutoresizingList, {
    List* l = newList();

    SomeObj* someObj;

    int n = 0;
    while (n < 10) {
        someObj = newSomeObj("Some String", (char* []) {"String 1", "String 2"});
        listAdd(l, (void**) &someObj);
        n++;
    }

    ASSERT_EQ_INT(10, l->count, "List should have 10 items in it");

    n = 0;
    SomeObj* retrieved;
    while (n < 10) {
        retrieved = (SomeObj*) l->values[n];
        ASSERT_EQ_STR("Some String", retrieved->someStr, "The `someStr` field should be correct");
        ASSERT_EQ_STR("String 1", retrieved->stringArr[0], "The 0th element of the `stringArr` field should match");
        ASSERT_EQ_STR("String 2", retrieved->stringArr[1], "The 1st element of the `stringArr` field should match");
        n++;
    }
})

void runListTests(Tester* tester) {
    tester->startSuite("List");
    tester->run(testCreateEmptyList);
    tester->run(testStoringAndRetrievingObject);
    tester->run(testAutoresizingList);
}
