#ifndef TIMETABLER_H_
#define TIMETABLER_H_

#include "matrix.h"

struct Class {
    int type;   // 0 = empty
    int room;   // < num_rooms
    int time;   // < 24
    int length; // < 24
};

struct TimeTable {
    TimeTable(int num_rooms) 
      : m(num_rooms * 24) {
        for (int i = 0; i < num_rooms * 24; ++i) {
            m.create_row            
        }
    }

    SparseMatrix m;
};

#endif
