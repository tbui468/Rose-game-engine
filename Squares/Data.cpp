#include "Data.h"

namespace sqs {

    static PuzzleData p00 {{1, 1}, {'r'}}; 
    static PuzzleData p01 {{2, 2}, {'r', 'g', 
                                          'b', 'e'}};
    static PuzzleData p02 {{4, 4}, {'e', 'g', 'r', 'r',
                                          'e', 'E', 'E', 'r',
                                          'e', 'r', 'g', 'r',
                                          'b', 'E', 'g', 'b'}};
    static PuzzleData p03 {{4, 3}, {'e', 'g', 'r', 'r',
                                          'e', 'E', 'E', 'r',
                                          'b', 'E', 'g', 'b'}};

    static PuzzleSetData ps0 {{p00, p01, p02, p03}};

    extern std::vector<PuzzleSetData> g_Data {ps0, ps0, ps0};


}
