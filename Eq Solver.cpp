#include <iostream>
#include <valarray>
#include <vector>
#include <sstream>
using namespace std;
struct Mat
{
	vector<valarray<float>> a;
	const int r, c;
	Mat(int r, int c, initializer_list<initializer_list<float>>_data = {}) :r(r), c(c) {
		a = vector<valarray<float>>(r);
		auto it = _data.begin();
		size_t i = 0;
		for (; it < _data.end() && i < a.size(); i++, it++) {
			a[i] = valarray<float>(c);
			memcpy(&a[i][0], it->begin(), it->size() * sizeof(float));
		}
		for (; i < a.size(); i++)
			a[i] = valarray<float>(c);

	}
	string ToStr()const {
		stringstream ss;
		for (int i = 0; i < a.size(); i++, ss << '\n')
			for (const float& c : a[i])
				ss << (c == 0 ? 0 : c) << " ";
		return ss.str();
	}
	operator string()const { return ToStr(); }

	valarray<float> Solve()
	{
		valarray<float> res(r);
		Mat ech = *this;
		bool hasSol = ech.CalcEch();
		_STL_VERIFY(hasSol, "No single solution for equation");
		for (int i = 0; i < ech.a.size(); i++) 
			res[i] = ech.a[i][c - 1];
		return res;
	}
private:
	//returns 1 if it has 1 sol
	bool CalcEch()
	{
		for (int i = 0; i < a.size() && i < c; i++)
		{
			if (a[i][i] == 0)
			{
				valarray<float>* d = GetNonZero(a, i);
				if (d == 0)
					return 0;
				swap(*d, a[i]);
			}
			a[i] /= float(a[i][i]);
			for (int j = 0; j < i; j++)
				Reduce(a[j], a[i], i);
			for (int j = i+1; j < a.size(); j++)
				Reduce(a[j], a[i], i);
		}
		return 1;
	}
private:
	valarray<float>* GetNonZero(vector<valarray<float>>& a, int idx)
	{
		for (int i = idx + 1; i < a.size(); i++)
			if (a[i][idx] )
				return &a[i];
		return 0;
	}
	void Reduce(valarray<float>& tgt, valarray<float>& src, int idx)
	{
		if (tgt[idx] == 0)
			return;
		float amnt = float(tgt[idx]) / float(src[idx]);
		tgt -= amnt * src;
	}
};
template<typename T>
void PrintA(const T& ar)
{
	for (int i = 0; i < ar.size(); i++)
		cout << ar[i] << " ";
	cout << "\n";
}
int main()
{
	Mat m(3, 4, {
		{ 2, 0,-6,-8 },
		{ 0, 12, 2, 3 },
		{ 0, 0,0,-4 },
		});
	
	auto res = m.Solve();
	PrintA(res);
}
