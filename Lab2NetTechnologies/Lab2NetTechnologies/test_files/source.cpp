#include <iostream> // i/o operations 
#include <fstream> // opening files
#include <string> // getline
#include <cmath> // trigon functions
#include "functions.h" // numerical methods
using std::cout;
using std::cin;

enum class funk_num {
    test_function,
    sin,
    cos,
    x,
    error_funk,
};

void network_menu()
{
    cout << "1: Get network from file" << '\n';
    cout << "2: Generate network from test function\n";
    cout << "3: Genereate network from sin x\n";
    cout << "4: Generate network from cos x\n";
    cout << "5: Generate network from x\n";
    cout << "0: Exit\n";
    cout << "Anything else: clear the concole\n";
}
// n - number of vertexes
void get_a_b_n(double& a, double& b,int& n)
{
    cout << "Enter the lower and upper limits of integration\n";
    cin >> a >> b;
    cout << "Enter the number of segments. The number should be 12k\n";
    do {
        cin >> n;
        if (n % 12 != 0 || n == 0 || n < 0) {
            cout << "Wrong number. Enter one more time\n";
        }
    } while (n % 12 != 0 || n == 0 || n < 0);
}
bool exit() 
{
    char nas = 0;
    cout << "If you are sure you want to exit, enter 0\n";
    cin >> nas;
    return (nas == '0') ? false : true;
}
void table_print(double num_res_Sim,
    double num_res_3_8,
    double num_res_Bool,
    double real_res,
    double mistake_Sim,
    double mistake_3_8,
    double mistake_Bool)
{
   /* cout << "METHOD";
    for (int i = 0; i < 6; i++) {
        cout << " ";
    }
    cout << "RESULT";
    for (int i = 0; i < 6; i++) {
        cout << " ";
    }
    cout << "DIFFERENCE\n";
    for(int i=)*/

    cout << "\nSIMPSON'S RULE\n";
    cout << "Result: " << num_res_Sim << "\n";
    cout << "Difference: " << mistake_Sim << "\n";
    cout << "\nSIMPSON'S 3/8 RULE\n";
    cout << "Result: " << num_res_3_8 << "\n";
    cout << "Difference: " << mistake_3_8 << "\n";
    cout << "\nBOOL'S RULE\n";
    cout << "Result: " << num_res_Bool << "\n";
    cout << "Difference: " << mistake_Bool << "\n\n";
    cout << "REAL RESULT\n";
    cout << "Result: " << real_res << "\n\n";
    system("pause");
}
void count_integrals(const vector<double>& x, const vector<double>&y, int segment_num, funk_num number)
{
    double num_res_Sim;
    double num_res_3_8;
    double num_res_Bool;
    double real_res;
    double mistake_Sim;
    double mistake_3_8;
    double mistake_Bool;

    num_res_Sim = full_Simpson_rule(x, y, segment_num);
    num_res_3_8 = full_Simpson_3_8_rule(x, y, segment_num);
    num_res_Bool = full_Bool_rule(x, y, segment_num);

    switch (number)
    {
    case funk_num::test_function:
        real_res = integral_test_8_polinom(x[0], x[x.size() - 1]);
        break;
    case funk_num::sin:
        real_res = integral_sin(x[0], x[x.size() - 1]);
        break;
    case funk_num::cos:
        real_res = integral_cos(x[0], x[x.size() - 1]);
        break;
    case funk_num::x:
        real_res = integral_x(x[0], x[x.size() - 1]);
        break;
    default:
        throw "Error in function number\n";
        break;
    }

    mistake_Sim = real_res - num_res_Sim;
    mistake_3_8 = real_res - num_res_3_8;
    mistake_Bool = real_res - num_res_Bool;
    table_print(num_res_Sim, num_res_3_8, num_res_Bool, real_res, mistake_Sim, mistake_3_8, mistake_Bool);
}
void menu()
{
    std::string line;
    vector<double> x;
    vector<double>y;
    double a = 0;
    double b = 25;
    int n = 24;
    char ans;
    bool not_exit = true;
    do {
        network_menu();
        std::cin >> ans;
        switch (ans)
        {
        case '1':
        {
            cout << "Press the path to file without spaces\n";
            cout << "There should be alternating x's and y's in the file\n";
            std::cin >> line;
            try {
                std::ifstream in(line); // окрываем файл для чтения 
                if (in.is_open())
                {
                    for (int i = 0; i < 25; i++) {
                        getline(in, line);
                        x.push_back(std::stod(line));
                        getline(in, line);
                        y.push_back(std::stod(line));
                    }
                }
                in.close();
                count_integrals(x, y, 25, funk_num::test_function);
            }
            catch (...) {
                cout << "Sorry, file did not found\n";
            }
        }
            break;
        case '2':
        {
            get_a_b_n(a, b, n);
            for (int i = 0; i <= n; i++) {
                x.push_back( a + ((b - a) / n * i));
                y.push_back(test_8_polinom(x[i]));
            }
            count_integrals(x, y, n, funk_num::test_function);
        }
            break;
        case '3':
        {
            get_a_b_n(a, b, n);
            for (int i = 0; i <= n; i++) {
                x.push_back(a + ((b - a) / n * i));
                y.push_back(sin(x[i]));
            }
            count_integrals(x, y, n, funk_num::sin);
        }
            break;
        case '4':
        {
            get_a_b_n(a, b, n);
            for (int i = 0; i <= n; i++) {
                x.push_back(a + ((b - a) / n * i));
                y.push_back(cos(x[i]));
            }
            count_integrals(x, y, n, funk_num::cos);
        }
            break;
        case '5':
        {
            get_a_b_n(a, b, n);
            for (int i = 0; i <= n; i++) {
                x.push_back(a + ((b - a) / n * i));
                y.push_back((x[i]));
            }
            count_integrals(x, y, n, funk_num::x);
        }
        break;
        case '0':
        {
            not_exit = exit();
        }
            break;
        default:
            system("cls");
            break;
        }
        x.clear();
        y.clear();
    } while (not_exit);
}

int main() 
{
    menu();
    return 0;
}