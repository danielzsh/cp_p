int -> long long
struct grazing:
	int x, y, t

main():
	input: g n (ints)
	[]cmp(grazing a, grazing b):
		return a.t < b.t
	set<grazing, cmp> grazings
	for i in [0, g):
		input: x y t (int)
		grazings.insert(grazing(x, y, t))
	int ans = 0
	for i in [0, n):
		input x y t (int)
		auto nxt = grazings.lower_bound(grazing(MIN, MIN, t))
		bool inncoent = false
		if nxt != grazings.end():
			int gx = nxt->x, gy = nxt->y, gt = nxt->t
			innocent = innocent || (SQUARE(gx - x) + SQUARE(gy - y) > SQUARE(gt - t))
		if nxt != grazings.begin():
			nxt = prev(nxt)
			int gx = nxt->x, gy = nxt->y, gt = nxt->t
			innocent = innocent || (SQUARE(gx - x) + SQUARE(gy - y) > SQUARE(gt - t))
		ans += innocent;
	print(ans)