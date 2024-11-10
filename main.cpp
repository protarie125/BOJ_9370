#include <algorithm>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

using ll = int64_t;
using vl = vector<ll>;
using vb = vector<bool>;

class Node {
 public:
  ll y;
  ll w;
};

using vN = vector<Node>;
using Adj = vector<vN>;

constexpr auto kInf = numeric_limits<ll>::max() / 2 - 1;

ll T;
ll n, m, t;
ll s, g, h;
Adj adj;

vl odist, xdist;
vb visit;

ll GetNearest(vl& dist) {
  ll ret = -1;
  ll mn = kInf;
  for (auto i = 1; i <= n; ++i) {
    if (visit[i]) continue;

    if (dist[i] < mn) {
      ret = i;
      mn = dist[i];
    }
  }

  return ret;
}

void Dist(ll p, vl& dist) {
  dist = vl(n + 1, kInf);
  visit = vb(n + 1, false);

  using pll = pair<ll, ll>;
  auto pq = priority_queue<pll, vector<pll>, greater<pll>>{};
  pq.push({0, p});

  while (!pq.empty()) {
    auto [w, nearest] = pq.top();
    pq.pop();
    if (dist[nearest] < w) continue;

    dist[nearest] = w;
    visit[nearest] = true;

    for (const auto& node : adj[nearest]) {
      if (visit[node.y]) continue;

      const auto& new_dist = dist[nearest] + node.w;
      if (new_dist < dist[node.y]) {
        dist[node.y] = new_dist;
        pq.push({new_dist, node.y});
      }
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  cin >> T;
  while (0 < (T--)) {
    cin >> n >> m >> t;
    cin >> s >> g >> h;

    adj = Adj(n + 1, vN{});
    ll w = 0;
    for (auto i = 0; i < m; ++i) {
      ll a, b, d;
      cin >> a >> b >> d;
      if (a == g && b == h) {
        w = d;
      } else if (a == h && b == g) {
        w = d;
      }

      adj[a].push_back(Node{b, d});
      adj[b].push_back(Node{a, d});
    }

    vl ans{};
    for (auto i = 0; i < t; ++i) {
      ll x;
      cin >> x;

      Dist(s, odist);
      Dist(x, xdist);
      // direct
      const auto& dist1 = odist[x];

      // smelt
      const auto& dist21 = odist[g] + w + xdist[h];
      const auto& dist22 = odist[h] + w + xdist[g];
      const auto& dist2 = min(dist21, dist22);

      if (dist2 <= dist1) {
        ans.push_back(x);
      }
    }

    sort(ans.begin(), ans.end());

    for (const auto& x : ans) {
      cout << x << ' ';
    }
    cout << '\n';
  }

  return 0;
}