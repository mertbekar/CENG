n = 38416;
k = 38416;

function w = oneWeight(x)
	if(x>=0 && x <= 1)
		w = ((0.4)*(x^3)) - ((0.6)*(x^2)) + 0.3;
	elseif(x>1 && x <= 2)
		w = ((-1.2)*((x-1)^3)) + (1.8 * ((x-1)^2)) + 0.1;
	elseif(x>2 && x <= 3)
		w = ((1.2)*((x-2)^3)) + ((-1.8)*((x-2)^2)) + 0.7;
	else
		w = 0;
	end
end






cc = 0;
allTotal = 0;
ar = zeros(38417:1);
it=1;
for p=1:n
	total = 0;
	count = 0;
	count = poissrnd(12);
	for h=1:count
		a = 0;
		b = 3;
		c = 0.7;
		X = 0;
		Y = c;
		weight = 0;

		while(Y > oneWeight(X))
			u = rand;
			v = rand;
			X = a + (b-a)*u;
			Y = c*v;
			end
		weight = X;
		total = total + weight;
		end
	allTotal = total + allTotal;
	ar(it) = total;
	it = it+1;
	if(total > 25)
		cc = cc+1;
		end
end
answer_1 = cc/n;
answer_1
at = allTotal/n;
answer_2 = allTotal/n;
answer_2
nn = k-1;
su = 0;
it = it - 1;
while(it>0)
	su = ((ar(it)-at)^2) + su;
	it = it-1;
	end
answer_3 = sqrt(su/nn);
answer_3
	
	
