#include "ls.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Test result tracking
int NUM_TCS = 12;
int* scores = NULL;
int tests_passed = 0;
int tests_failed = 0;

// Compare two StringMatrix structures
bool are_matrices_equal(const StringMatrix* mat1, const StringMatrix* mat2) {
    if (mat1 == NULL || mat2 == NULL) {
        return mat1 == mat2;
    }
    
    if (mat1->rows != mat2->rows) {
        return false;
    }
    
    for (size_t i = 0; i < mat1->rows; ++i) {
        if (mat1->cols[i] != mat2->cols[i]) {
            return false;
        }
        
        for (size_t j = 0; j < mat1->cols[i]; ++j) {
            const char* str1 = (mat1->data != NULL && mat1->data[i] != NULL) ? mat1->data[i][j] : NULL;
            const char* str2 = (mat2->data != NULL && mat2->data[i] != NULL) ? mat2->data[i][j] : NULL;
            
            if (str1 == NULL || str2 == NULL) {
                if (str1 != str2) {
                    return false;
                }
            } else if (strcmp(str1, str2) != 0) {
                return false;
            }
        }
    }
    
    return true;
}

// Create expected StringMatrix from test data
StringMatrix* create_expected_matrix(const char** expected_data, size_t rows, size_t* cols_per_row) {
    if (expected_data == NULL || cols_per_row == NULL) {
        return NULL;
    }
    
    StringMatrix* matrix = (StringMatrix*)malloc(sizeof(StringMatrix));
    if (matrix == NULL) {
        return NULL;
    }
    
    matrix->rows = rows;
    matrix->cols = (size_t*)malloc(rows * sizeof(size_t));
    if (matrix->cols == NULL) {
        free(matrix);
        return NULL;
    }
    
    matrix->data = (char***)malloc(rows * sizeof(char**));
    if (matrix->data == NULL) {
        free(matrix->cols);
        free(matrix);
        return NULL;
    }
    
    size_t total_strings_needed = 0;
    for (size_t i = 0; i < rows; ++i) {
        total_strings_needed += cols_per_row[i];
    }
    
    size_t data_idx = 0;
    for (size_t i = 0; i < rows; ++i) {
        matrix->cols[i] = cols_per_row[i];
        matrix->data[i] = (char**)malloc(cols_per_row[i] * sizeof(char*));
        if (matrix->data[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                for (size_t k = 0; k < matrix->cols[j]; ++k) {
                    free(matrix->data[j][k]);
                }
                free(matrix->data[j]);
            }
            free(matrix->data);
            free(matrix->cols);
            free(matrix);
            return NULL;
        }
        
        for (size_t j = 0; j < cols_per_row[i]; ++j) {
            if (data_idx >= total_strings_needed) {
                for (size_t k = 0; k < j; ++k) {
                    free(matrix->data[i][k]);
                }
                free(matrix->data[i]);
                for (size_t k = 0; k < i; ++k) {
                    for (size_t l = 0; l < matrix->cols[k]; ++l) {
                        free(matrix->data[k][l]);
                    }
                    free(matrix->data[k]);
                }
                free(matrix->data);
                free(matrix->cols);
                free(matrix);
                return NULL;
            }
            
            if (expected_data[data_idx] != NULL) {
                size_t len = strlen(expected_data[data_idx]) + 1;
                matrix->data[i][j] = (char*)malloc(len);
                if (matrix->data[i][j] == NULL) {
                    for (size_t k = 0; k < j; ++k) {
                        free(matrix->data[i][k]);
                    }
                    free(matrix->data[i]);
                    for (size_t k = 0; k < i; ++k) {
                        for (size_t l = 0; l < matrix->cols[k]; ++l) {
                            free(matrix->data[k][l]);
                        }
                        free(matrix->data[k]);
                    }
                    free(matrix->data);
                    free(matrix->cols);
                    free(matrix);
                    return NULL;
                }
                strcpy(matrix->data[i][j], expected_data[data_idx]);
            } else {
                matrix->data[i][j] = NULL;
            }
            data_idx++;
        }
    }
    
    return matrix;
}

// Run a test
void run_test(const char* test_name, bool (*test_func)(void)) {
    printf("Running test: %s\n", test_name);
    bool passed = test_func();
    if (passed) {
        printf("  PASSED\n");
        tests_passed++;
    } else {
        printf("  FAILED\n");
        tests_failed++;
    }
}

bool test_basic(void) {
    Ls* ls = ls_create_with_flags(false, false, false);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../data/bengali",
        "../data/hindi",
        "../data/marathi",
        "../data/namaste.txt"
    };
    size_t cols_per_row[] = {1, 1, 1, 1};
    StringMatrix* expected = create_expected_matrix(expected_data, 4, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[0] = passed ? 10 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_l_flag(void) {
    Ls* ls = ls_create_with_flags(false, true, false);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../data/bengali", "DIRECTORY",
        "../data/hindi", "DIRECTORY",
        "../data/marathi", "DIRECTORY",
        "../data/namaste.txt", "FILE"
    };
    size_t cols_per_row[] = {2, 2, 2, 2};
    StringMatrix* expected = create_expected_matrix(expected_data, 4, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[1] = passed ? 10 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_a_flag(void) {
    Ls* ls = ls_create_with_flags(true, false, false);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../data/.",
        "../data/..",
        "../data/bengali",
        "../data/hindi",
        "../data/marathi",
        "../data/namaste.txt"
    };
    size_t cols_per_row[] = {1, 1, 1, 1, 1, 1};
    StringMatrix* expected = create_expected_matrix(expected_data, 6, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[2] = passed ? 10 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_h_flag(void) {
    Ls* ls = ls_create_with_flags(false, false, true);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../data/bengali/namaskaram_hardlink.txt",
        "../data/hindi/kannada/namaskaram.txt",
        "../data/hindi/tamil/namaskaram.txt",
        "../data/hindi/vanakkam.txt",
        "../data/marathi/vanakkam_hardlink.txt",
        "../data/marathi/assamese/namaste_hardlink.txt",
        "../data/namaste.txt"
    };
    size_t cols_per_row[] = {3, 2, 2};
    StringMatrix* expected = create_expected_matrix(expected_data, 3, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[3] = passed ? 20 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_lh_flag(void) {
    Ls* ls = ls_create_with_flags(false, true, true);
    if (ls == NULL) return false;
    
    // With lh flags, hardlinks are grouped with file type at the end
    const char* expected_data[] = {
        "../data/bengali/namaskaram_hardlink.txt",
        "../data/hindi/kannada/namaskaram.txt",
        "../data/hindi/tamil/namaskaram.txt",
        "FILE",
        "../data/hindi/vanakkam.txt",
        "../data/marathi/vanakkam_hardlink.txt",
        "FILE",
        "../data/marathi/assamese/namaste_hardlink.txt",
        "../data/namaste.txt",
        "FILE"
    };
    size_t cols_per_row[] = {4, 3, 3};
    StringMatrix* expected = create_expected_matrix(expected_data, 3, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[4] = passed ? 20 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_ah_flag(void) {
    Ls* ls = ls_create_with_flags(true, false, true);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../data/.",
        "../data/bengali/..",
        "../data/hindi/..",
        "../data/marathi/..",
        "../data/bengali",
        "../data/bengali/.",
        "../data/bengali/malayalam/..",
        "../data/bengali/malayalam",
        "../data/bengali/malayalam/.",
        "../data/bengali/malayalam/swagatham.txt",
        "../data/hindi/tamil/telugu/.malayalam_swagatham_hardlink.txt",
        "../data/bengali/namaskaram_hardlink.txt",
        "../data/hindi/kannada/.namaskaram_hardlink.txt",
        "../data/hindi/kannada/namaskaram.txt",
        "../data/hindi/tamil/namaskaram.txt",
        "../data/hindi",
        "../data/hindi/.",
        "../data/hindi/kannada/..",
        "../data/hindi/tamil/..",
        "../data/hindi/kannada",
        "../data/hindi/kannada/.",
        "../data/hindi/tamil",
        "../data/hindi/tamil/.",
        "../data/hindi/tamil/telugu/..",
        "../data/hindi/tamil/.hidden_file1",
        "../data/hindi/tamil/telugu/kannada_swagatha_hardlink.txt",
        "../data/hindi/tamil/telugu",
        "../data/hindi/tamil/telugu/.",
        "../data/hindi/vanakkam.txt",
        "../data/marathi/vanakkam_hardlink.txt",
        "../data/marathi",
        "../data/marathi/.",
        "../data/marathi/assamese/..",
        "../data/marathi/assamese",
        "../data/marathi/assamese/.",
        "../data/marathi/assamese/namaste_hardlink.txt",
        "../data/namaste.txt"
    };
    // Rows have varying column counts: directories with . and .., files with hardlinks grouped
    // Updated to match actual output: 14 rows (directories grouped, hardlinks grouped, hidden files with hardlinks grouped)
    // Row order: root dir, bengali dir, malayalam dir, swagatham hardlinks, namaskaram hardlinks, hindi dir, kannada dir, tamil dir, hidden_file1 hardlinks, telugu dir, vanakkam hardlinks, marathi dir, assamese dir, namaste hardlinks
    size_t cols_per_row[] = {4, 3, 2, 2, 4, 4, 2, 3, 2, 2, 2, 3, 2, 2};
    StringMatrix* expected = create_expected_matrix(expected_data, 14, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[5] = passed ? 20 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_al_flag(void) {
    Ls* ls = ls_create_with_flags(true, true, false);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../data/.", "DIRECTORY",
        "../data/..", "DIRECTORY",
        "../data/bengali", "DIRECTORY",
        "../data/hindi", "DIRECTORY",
        "../data/marathi", "DIRECTORY",
        "../data/namaste.txt", "FILE"
    };
    size_t cols_per_row[] = {2, 2, 2, 2, 2, 2};
    StringMatrix* expected = create_expected_matrix(expected_data, 6, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[6] = passed ? 30 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_alh_flag(void) {
    Ls* ls = ls_create_with_flags(true, true, true);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../data/.",
        "../data/bengali/..",
        "../data/hindi/..",
        "../data/marathi/..",
        "DIRECTORY",
        "../data/bengali",
        "../data/bengali/.",
        "../data/bengali/malayalam/..",
        "DIRECTORY",
        "../data/bengali/malayalam",
        "../data/bengali/malayalam/.",
        "DIRECTORY",
        "../data/bengali/malayalam/swagatham.txt",
        "../data/hindi/tamil/telugu/.malayalam_swagatham_hardlink.txt",
        "FILE",
        "../data/bengali/namaskaram_hardlink.txt",
        "../data/hindi/kannada/.namaskaram_hardlink.txt",
        "../data/hindi/kannada/namaskaram.txt",
        "../data/hindi/tamil/namaskaram.txt",
        "FILE",
        "../data/hindi",
        "../data/hindi/.",
        "../data/hindi/kannada/..",
        "../data/hindi/tamil/..",
        "DIRECTORY",
        "../data/hindi/kannada",
        "../data/hindi/kannada/.",
        "DIRECTORY",
        "../data/hindi/tamil",
        "../data/hindi/tamil/.",
        "../data/hindi/tamil/telugu/..",
        "DIRECTORY",
        "../data/hindi/tamil/.hidden_file1",
        "../data/hindi/tamil/telugu/kannada_swagatha_hardlink.txt",
        "FILE",
        "../data/hindi/tamil/telugu",
        "../data/hindi/tamil/telugu/.",
        "DIRECTORY",
        "../data/hindi/vanakkam.txt",
        "../data/marathi/vanakkam_hardlink.txt",
        "FILE",
        "../data/marathi",
        "../data/marathi/.",
        "../data/marathi/assamese/..",
        "DIRECTORY",
        "../data/marathi/assamese",
        "../data/marathi/assamese/.",
        "DIRECTORY",
        "../data/marathi/assamese/namaste_hardlink.txt",
        "../data/namaste.txt",
        "FILE"
    };
    // Rows have varying columns: directories with . and .. + type, files with hardlinks + type
    // Updated to match actual output: 14 rows
    // Row order: root(5), bengali(4), malayalam(3), swagatham(3), namaskaram(5), hindi(5), kannada(3), tamil(4), hidden_file1(3), telugu(3), vanakkam(3), marathi(4), assamese(3), namaste(3)
    size_t cols_per_row[] = {5, 4, 3, 3, 5, 5, 3, 4, 3, 3, 3, 4, 3, 3};
    StringMatrix* expected = create_expected_matrix(expected_data, 14, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[7] = passed ? 50 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_ah_hidden(void) {
    Ls* ls = ls_create_with_flags(true, false, true);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../hidden_data/etc/.",
        "../hidden_data/etc/pacman/..",
        "../hidden_data/etc/pacman",
        "../hidden_data/etc/pacman/."
    };
    // Only directories grouped (no files with hardlinks in etc directory)
    size_t cols_per_row[] = {2, 2};
    StringMatrix* expected = create_expected_matrix(expected_data, 2, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../hidden_data/etc");
    bool passed = are_matrices_equal(result, expected);
    
    scores[8] = passed ? 50 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_lh_hidden(void) {
    Ls* ls = ls_create_with_flags(false, true, true);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../hidden_data/bin/cat.exe",
        "../hidden_data/home/abc/cat",
        "FILE",
        "../hidden_data/bin/ls",
        "../hidden_data/etc/ls_etc",
        "FILE",
        "../hidden_data/swapfile",
        "../hidden_data/swapfile_pc",
        "FILE"
    };
    size_t cols_per_row[] = {3, 3, 3};
    StringMatrix* expected = create_expected_matrix(expected_data, 3, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../hidden_data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[9] = passed ? 50 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_al_hidden(void) {
    Ls* ls = ls_create_with_flags(true, true, false);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../hidden_data/home/abc/.", "DIRECTORY",
        "../hidden_data/home/abc/..", "DIRECTORY",
        "../hidden_data/home/abc/.bash_history", "FILE",
        "../hidden_data/home/abc/.cache", "DIRECTORY",
        "../hidden_data/home/abc/Documents", "DIRECTORY",
        "../hidden_data/home/abc/Downloads", "DIRECTORY",
        "../hidden_data/home/abc/RanDOM.c", "FILE",
        "../hidden_data/home/abc/cat", "FILE",
        "../hidden_data/home/abc/hello.cpp", "SOFTLINK",
        "../hidden_data/home/abc/ls", "SOFTLINK"
    };
    size_t cols_per_row[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    StringMatrix* expected = create_expected_matrix(expected_data, 10, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../hidden_data/home/abc");
    bool passed = are_matrices_equal(result, expected);
    
    scores[10] = passed ? 50 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

bool test_alh_hidden(void) {
    Ls* ls = ls_create_with_flags(true, true, true);
    if (ls == NULL) return false;
    
    const char* expected_data[] = {
        "../hidden_data/.",
        "../hidden_data/bin/..",
        "../hidden_data/etc/..",
        "../hidden_data/home/..",
        "DIRECTORY",
        "../hidden_data/bin",
        "../hidden_data/bin/.",
        "DIRECTORY",
        "../hidden_data/bin/cat.exe",
        "../hidden_data/home/abc/cat",
        "FILE",
        "../hidden_data/bin/ls",
        "../hidden_data/etc/ls_etc",
        "../hidden_data/home/abc/Downloads/.ls",
        "FILE",
        "../hidden_data/etc",
        "../hidden_data/etc/.",
        "../hidden_data/etc/pacman/..",
        "DIRECTORY",
        "../hidden_data/etc/pacman",
        "../hidden_data/etc/pacman/.",
        "DIRECTORY",
        "../hidden_data/home",
        "../hidden_data/home/.",
        "../hidden_data/home/abc/..",
        "DIRECTORY",
        "../hidden_data/home/abc",
        "../hidden_data/home/abc/.",
        "../hidden_data/home/abc/.cache/..",
        "../hidden_data/home/abc/Documents/..",
        "../hidden_data/home/abc/Downloads/..",
        "DIRECTORY",
        "../hidden_data/home/abc/.cache",
        "../hidden_data/home/abc/.cache/.",
        "DIRECTORY",
        "../hidden_data/home/abc/.cache/jdk",
        "../hidden_data/home/abc/.cache/jdk1",
        "FILE",
        "../hidden_data/home/abc/Documents",
        "../hidden_data/home/abc/Documents/.",
        "../hidden_data/home/abc/Documents/temp/..",
        "DIRECTORY",
        "../hidden_data/home/abc/Documents/temp",
        "../hidden_data/home/abc/Documents/temp/.",
        "DIRECTORY",
        "../hidden_data/home/abc/Documents/temp/.swapfile",
        "../hidden_data/swapfile",
        "../hidden_data/swapfile_pc",
        "FILE",
        "../hidden_data/home/abc/Downloads",
        "../hidden_data/home/abc/Downloads/.",
        "DIRECTORY"
    };
    size_t cols_per_row[] = {5, 3, 3, 4, 4, 3, 4, 6, 3, 3, 4, 3, 4, 3};
    StringMatrix* expected = create_expected_matrix(expected_data, 14, cols_per_row);
    if (expected == NULL) {
        ls_destroy(ls);
        return false;
    }
    
    StringMatrix* result = ls_run(ls, "../hidden_data");
    bool passed = are_matrices_equal(result, expected);
    
    scores[11] = passed ? 100 : 0;
    
    string_matrix_destroy(result);
    string_matrix_destroy(expected);
    ls_destroy(ls);
    return passed;
}

// Main test runner
int main(void) {
    printf("Starting ls tests...\n\n");
    
    scores = (int*)calloc(NUM_TCS, sizeof(int));
    if (scores == NULL) {
        fprintf(stderr, "Failed to allocate scores array\n");
        return 1;
    }
    
    run_test("Basic", test_basic);
    run_test("l flag", test_l_flag);
    run_test("a flag", test_a_flag);
    run_test("h flag", test_h_flag);
    run_test("lh flag", test_lh_flag);
    run_test("ah flag", test_ah_flag);
    run_test("al flag", test_al_flag);
    run_test("alh flag", test_alh_flag);
    run_test("ah hidden", test_ah_hidden);
    run_test("lh hidden", test_lh_hidden);
    run_test("al hidden", test_al_hidden);
    run_test("alh hidden", test_alh_hidden);
    
    printf("\n=== Test Summary ===\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("Total tests: %d\n", tests_passed + tests_failed);
    
    int total_score = 0;
    for (int i = 0; i < NUM_TCS; ++i) {
        total_score += scores[i];
    }
    printf("Total score: %d\n", total_score);
    
    FILE* fp = fopen("test_results.json", "w");
    if (fp != NULL) {
        fprintf(fp, "{\"_presentation\":\"semantic\"}\n");
        fprintf(fp, "{\"scores\":{");
        for (int i = 0; i < NUM_TCS; ++i) {
            fprintf(fp, "\"test_case_%d\": %d%s", i + 1, scores[i], (i == NUM_TCS - 1 ? "" : ","));
        }
        fprintf(fp, "}}");
        fclose(fp);
    }
    
    free(scores);
    
    return (tests_failed == 0) ? 0 : 1;
}