#include "Data.h"

namespace sqs {

    static PuzzleData p00 {{1, 1}, {'r'}, 3}; 
    static PuzzleData p01 {{2, 2}, {'r', 'R', 
                                    'b', 'e'}, 3};
    static PuzzleData p02 {{6, 4}, {'E', 'g', 'g', 'g', 'g', 'G',
                                    'E', 'e', 'e', 'e', 'r', 'E',
                                    'G', 'r', 'g', 'G', 'g', 'E',
                                    'E', 'g', 'e', 'g', 'r', 'E'}, 10};
    static PuzzleData p03 {{4, 3}, {'B', 'g', 'r', 'r',
                                    'e', 'E', 'E', 'r',
                                    'b', 'E', 'g', 'b'}, 5};

    static PuzzleSetData ps0 {{p00, p01, p02, p03}};

    extern std::vector<PuzzleSetData> g_Data {ps0, ps0, ps0}; //puzzle data is loaded into this vector, and all user interactive writes into here
    const extern std::vector<PuzzleSetData> g_DefaultData {g_Data};


}
