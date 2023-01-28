#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <queue>

using namespace std;

typedef struct _params
{
    int id;
    int R;
    int M;

    int no_of_files;
    string test_file;
    queue<string> *files_to_be_open;
    map<int, vector<int>> *m;

    pthread_barrier_t *barrier;
    pthread_mutex_t *mutex;

} params;


void get_args(int argc, char **argv, params *par)
{
    if (argc < 4)
    {
        printf("Numar insuficient de parametri:\n\t"
               "./tema1 <numar_mapperi> <numar_reduceri> <fisier_intrare>\n");
        exit(1);
    }

    par->M = atoi(argv[1]);
    par->R = atoi(argv[2]);
    par->test_file = argv[3];
}

void read_input_file(params *par, queue<string> *files_to_be_open)
{
    ifstream inTest;
    inTest.open(par->test_file.c_str());

    if (inTest.is_open())
    {
        inTest >> par->no_of_files;
        string files;
        for (int i = 0; i < par->no_of_files; i++)
        {
            inTest >> files;
            (*files_to_be_open).push(files);
        }
        par->files_to_be_open = files_to_be_open;
        inTest.close();
    }
}

void write_output_file(int number, int res)
{
    fstream out;
    string name_file = "out";
    string nr = to_string(number);
    name_file += nr;
    name_file += ".txt";

    out.open(name_file, ios::out);
    out << res;

    out.close();
}


int binary_search(int number, int n)
{
    if (number < 2)
    {
        return number;
    }

    int left = 0;
    int right = sqrt(number);
    int mid;
    int res = 0;

    while (left <= right)
    {
        mid = left + (right - left) / 2;
        if (pow(mid, n) == number)
        {
            res = mid;
            break;
        }
        else if (pow(mid, n) < number)
        {
            left = mid + 1;
            res = mid;
        }
        else
        {
            right = mid - 1;
        }
    }
    if (pow(res, n) == number)
    {
        return res;
    }
    return 0;
}


void *mapper(void *arg)
{
    params data = *(params *)arg;

    while (!(*(data.files_to_be_open)).empty())
    {
        ifstream inFile;
        pthread_mutex_lock(&(*data.mutex));

        if ((*(data.files_to_be_open)).empty()) {
            pthread_mutex_unlock(&(*data.mutex));
            break;
        }
        inFile.open((*(data.files_to_be_open)).front());
        (*(data.files_to_be_open)).pop();

        pthread_mutex_unlock(&(*data.mutex));

        if (inFile.is_open())
        {
            int n;
            inFile >> n;
            for (int i = 0; i < n; i++) {
                int number;
                inFile >> number;
                for (int j = 2; j <= data.R + 1; j++) {
                    if (binary_search(number, j)) {
                        (*data.m)[j].push_back(number);
                    }
                }
            }
            inFile.close();
        }
    }


    pthread_barrier_wait(&(*data.barrier));

    pthread_exit(NULL);
}

void *reducer(void *arg) {
    params data = *(params *) arg;

    pthread_barrier_wait(&(*data.barrier));

    set<int> reducer;
    map<int, vector<int>>::iterator itr;
    for (itr = (*data.m).begin(); itr != (*data.m).end(); ++itr)
    {
        for (int i = 0; i < (int)itr->second.size(); i++)
        {
            reducer.insert(itr->second[i]);
        }
        write_output_file(itr->first, reducer.size());
        reducer.clear();
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    params par;
    pthread_barrier_t barrier;
    pthread_mutex_t mutex;
    int i, r;
    void *status;

    map<int, vector<int>> m;
    queue<string> q;

    get_args(argc, argv, &par);
    pthread_t threads[par.M + par.R];
    params arguments[par.M + par.R];

    read_input_file(&par, &q);

    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        cout << "Error to initialize mutex";
        return 1;
    }

    if (pthread_barrier_init(&barrier, NULL, par.M + par.R) != 0)
    {
        printf("Error to initialize barrier");
        return 1;
    }

    for (i = 0; i < par.M + par.R; i++)
    {
        arguments[i].id = i;
        arguments[i].M = par.M;
        arguments[i].R = par.R;

        arguments[i].test_file = par.test_file;
        arguments[i].files_to_be_open = &q;
        arguments[i].no_of_files = par.no_of_files;
        arguments[i].m = &m;

        arguments[i].barrier = &barrier;
        arguments[i].mutex = &mutex;

        if( i < par.M) {
            r = pthread_create(&threads[i], NULL, mapper, &arguments[i]);
        } else  {
            r = pthread_create(&threads[i], NULL, reducer, &arguments[i]);
        }

        if (r)
        {
            printf("Eroare la crearea thread-ului %d\n", i);
            exit(-1);
        }
    }

    for (i = 0; i < par.M + par.R; i++)
    {
        r = pthread_join(threads[i], &status);

        if (r)
        {
            printf("Eroare la asteptarea thread-ului %d\n", i);
            exit(-1);
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);

    return 0;
}


