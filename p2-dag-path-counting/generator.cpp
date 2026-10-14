/**********************************************************************
 * IST - ASA 25/26 - Projecto 2 - Instance Generator - Pedro Monteiro *
 **********************************************************************/
#include <iostream>
#include <vector>
using namespace std;

// Dimensions and Params
int _N, _M, _D;
struct Edge { int u, v; };

//-----------------------------------------------------------------------------

void printUsage(char *progname) {
  cerr << "Usage: " << progname << " <N> <M> <Density> <seed>" << endl;
  cerr << "  N: number of intersections (nodes) >= 2" << endl;
  cerr << "  M: number of trucks >= 2" << endl;
  cerr << "  Density: edge probability (0-100)" << endl;
  cerr << "  seed: random seed number (optional)" << endl;
  exit(1);
}

void parseArgs(int argc, char *argv[]) {
  int seed = 0;

  if (argc < 4 || argc > 5) {
    cerr << "ERROR: Wrong number of arguments" << endl;
    printUsage(argv[0]);
  }

  sscanf(argv[1], "%d", &_N);
  if (_N < 2) {
    cerr << "ERROR: N must be >= 2" << endl;
    printUsage(argv[0]);
  }

  sscanf(argv[2], "%d", &_M);
  if (_M < 2) {
    cerr << "ERROR: M must be >= 2" << endl;
    printUsage(argv[0]);
  }

  sscanf(argv[3], "%d", &_D);
  if (_D < 0 || _D > 100) {
    cerr << "ERROR: Density must be between 0 and 100" << endl;
    printUsage(argv[0]);
  }

  if (argc == 5) {
    sscanf(argv[4], "%d", &seed);
    srand(seed);
  } else { // pseudo-random seed
    srand((unsigned int)time(NULL));
  }
}

inline int randomValue(int max) {
  if (max == 0) return 0;
  return rand() % max; // [0, max - 1]
}

int main(int argc, char *argv[]) {
  parseArgs(argc, argv);

  // Print dimensions
  cout << _N << endl << _M << endl;
  // int m1 = 1 + randomValue(_M);
  // int m2 = m1 + randomValue(_M - m1 + 1);
  int m1 = 1, m2 = _N;
  if (m2 > _M) m2 = _M; 
  cout << m1 << " " << m2 << endl;

  // Generate graph
  vector<int> p(_N);
  for(int i=0; i<_N; ++i) p[i] = i + 1;
  for (int i = 0; i < _N; i++) {
    int r = randomValue(_N);
    int tmp = p[i];
    p[i] = p[r];
    p[r] = tmp;
  }

  int max_edges_DAG = _N * (_N - 1) / 2;
  int max_edges_planar = 3 * _N + 6;
  int num_edges = min(max_edges_planar, max_edges_DAG);
  num_edges = (num_edges * _D) / 100;

  vector<Edge> edges;
  vector<vector<bool>> adj_matrix(_N, vector<bool>(_N, false));

  int i = 0;
  while (i < num_edges)  {
	int a = randomValue(_N - 1);
	int b = a + 1 + randomValue(_N - a - 1);
	if(!adj_matrix[a][b]) {
	  edges.push_back({p[a], p[b]});
	  adj_matrix[a][b] = true;
	  i++;
	}
  }

  // Limit case
  if (edges.empty() && _N >= 2) edges.push_back({p[0], p[1]});
  // Print edges
  cout << edges.size() << endl;
  for (size_t i = 0; i < edges.size(); i++)
      cout << edges[i].u << " " << edges[i].v << endl;

  return 0;
}
