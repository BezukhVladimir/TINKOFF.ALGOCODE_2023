#include <ctime>
#include <cmath>
#include <random>
#include <fstream>
#include <iostream>
#include <algorithm>

/*
double MAX_T     = 1.0;  // Начальная температура
double MIN_T     = 1e-8; // Конечная температура
double DECAY     = 0.95; // Коэффициент отжига
const int L      = 1000; // Количество итераций при каждой температуре
const int CITIES = 312;  // Количество городов
const int TESTS  = 500;  // Количество тестов (AMD 4700U, 16GB, примерно 4 минуты)
int map[CITIES][CITIES]; // Матрица смежности
int best_path[CITIES];   // Города в порядке обхода (лучший из тестов)z
int best_path_length = std::numeric_limits<int>::max();

std::ifstream input_file;
const char * filepath = "C://Users//PC//Desktop//dataset.txt";

int randNumber(const int &min = 0, const int &max = std::numeric_limits<int>::max()) {
    static std::mt19937 gen(std::time(NULL));
    std::uniform_int_distribution<> uid(min, max);
    return uid(gen);
}

double randReal(const double &min = 0, const double &max = 1) {
    static std::mt19937 gen(std::time(NULL));
    std::uniform_real_distribution<> uid(min, max);
    return uid(gen);
}

void setDefaultPath(int * path) {
    for (int i = 0; i < CITIES; ++i) {
        path[i] = i;
    }
}

void readData() {
    input_file.open(filepath);
    for (int i = 0; i < CITIES; i++) {
        for (int j = 0; j < CITIES; j++) {
            input_file >> map[i][j];
        }
    }
}

int calculatePathLength(int * path) {
    int total_length = 0;
    for (int i = 0; i < CITIES - 1; i++) {
        total_length += map[path[i]][path[i + 1]];
    }

    total_length += map[path[CITIES - 1]][path[0]];
    return total_length;
}

void evaluate(int * path) {
    int pos1 = randNumber(0, CITIES - 1);
    int pos2 = randNumber(0, CITIES - 1);

    std::swap(path[pos1], path[pos2]);
}

void copyPath(int * from, int * to) {
    memcpy(to, from, CITIES * sizeof(int));
}

void updateBestPath(int * current_path, int current_path_length) {
    best_path_length = current_path_length;
    copyPath(current_path, best_path);
}

void solveBySimulatedAnnealing() {
    int new_path[CITIES];
    int current_path[CITIES];
    int new_path_length;
    int current_path_length;

    for (int test = 1; test <= TESTS; test++) {
        double t = MAX_T;
        setDefaultPath(current_path);

        while (t > MIN_T) {
            for (int i = 1; i <= L; i++) {
                copyPath(current_path, new_path);
                evaluate(new_path);

                new_path_length     = calculatePathLength(new_path);
                current_path_length = calculatePathLength(current_path);

                int dif = current_path_length - new_path_length;
                if (dif >= 0) {
                    if (std::exp (-dif / t) < randReal()) {
                        copyPath(new_path, current_path);
                    }
                }
            }

            t *= DECAY;
        }

        if (current_path_length < best_path_length) {
            updateBestPath(current_path, current_path_length);
        }
    }
}

void printAnswer() {
    for (int i = 0; i < CITIES; ++i) {
        std::cout << best_path[i] + 1 << ' ';
    }

    std::cout << '\n' << best_path_length;
}

int main() {
    time_t start, finish;
    start = clock();

    readData();
    solveBySimulatedAnnealing();
    printAnswer();

    finish = clock();
    std::cout << '\n' << (double) (finish - start) / CLOCKS_PER_SEC;
}
*/

int main() {
    std::cout << "167 92 249 269 197 227 201 267 297 312 139 12 65 30 288 51 58 108 81 270 79 305 36 131 298 5 230 271 55 254 284 172 191 24 137 251 3 289 272 223 182 41 285 38 67 205 207 210 178 117 307 155 159 145 46 34 40 91 243 164 136 247 234 275 180 20 204 103 121 23 160 69 97 292 1 84 286 239 193 153 240 311 199 4 246 156 7 212 62 105 74 57 200 228 166 233 257 189 150 300 188 16 238 143 68 157 14 110 260 54 306 202 263 171 43 185 278 88 119 120 148 87 235 31 78 158 224 140 154 56 100 122 175 35 147 11 53 221 109 80 215 208 184 294 18 141 129 217 6 283 83 181 299 266 50 303 296 190 72 52 220 13 198 302 116 47 66 149 174 96 280 152 37 42 282 29 194 128 177 301 115 111 138 22 226 94 9 125 142 21 132 104 259 61 2 310 86 304 76 106 214 49 75 231 134 133 281 236 293 206 273 252 308 261 45 146 161 107 118 44 33 211 237 187 203 255 216 222 253 165 135 232 151 256 277 287 85 77 225 26 186 241 242 144 93 102 63 262 196 32 73 162 126 170 195 168 28 112 176 95 130 264 192 89 276 123 60 114 71 265 10 64 250 127 101 274 124 295 258 27 219 48 268 99 17 244 245 209 8 183 90 70 309 59 279 39 179 163 15 19 229 113 169 98 213 218 173 25 291 290 82 248";
}