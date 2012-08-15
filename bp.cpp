#include<iostream>
#include<vector>
#include<cstdio>
#include<cmath>


using namespace std;

const int INPUT_NUM = 8;
const int MID_NUM = 3;
const int OUTPUT_NUM = INPUT_NUM;

vector<vector<double> > w12, w23;
vector<double> w0h, w0k;
vector<vector<double> > train_input, train_output;

void init_trainset()
{
	for (int i=0; i<8; i++) {
		vector<double> input(INPUT_NUM);
		input[i] = 1.0;
		train_input.push_back(input);
		input[i] = 0.9;
		train_output.push_back(input);
	}
	for (int i=0; i<train_input.size(); i++) {
		for (int j=0; j<train_input[i].size(); j++)
			printf("%.1f ", train_input[i][j]);
		printf("\n");
		for (int j=0; j<train_output[i].size(); j++)
			printf("%.1f ", train_output[i][j]);
		printf("\n");
	}
}

void init_net()
{
	for (int i=0; i<INPUT_NUM; i++)
		w12.push_back(vector<double>(MID_NUM));
	for (int i=0; i<MID_NUM; i++)
		w23.push_back(vector<double>(OUTPUT_NUM));

	w0h.resize(MID_NUM);
	w0k.resize(OUTPUT_NUM);

	double range = 1.0;
	for (int i=0; i<w12.size(); i++) {
		for (int j=0; j<w12[i].size(); j++)
			w12[i][j] = range * (rand() / (RAND_MAX + 1.0)) - range/2;
	}
	for (int i=0; i<w23.size(); i++) {
		for (int j=0; j<w23[i].size(); j++)
			w23[i][j] = range * (rand() / (RAND_MAX + 1.0)) - range/2;
	}
	for (int i=0; i<w0h.size(); i++)
		w0h[i] = range * (rand() / (RAND_MAX + 1.0)) - range/2;
	for (int i=0; i<w0k.size(); i++)
		w0k[i] = range * (rand() / (RAND_MAX + 1.0)) - range/2;

}

inline double sqr(double x)
{
	return x*x;
}

void train()
{
	vector<double> oks(OUTPUT_NUM);
	vector<double> ohs(MID_NUM);
	vector<double> dks(OUTPUT_NUM);
	vector<double> dhs(MID_NUM);
	double eta = 0.8;
	double scale = 0.9999;

	FILE *f_output_err = fopen("output_err.txt", "w");
	FILE *f_h_value = fopen("h_value.txt", "w");
	FILE *f_w12 = fopen("w12.txt", "w");

	vector<double> o_errs(oks.size());

	int iter_num = 3000;
	for (int iter=0; iter<iter_num; iter++) {
		double tot_err = 0.0;
		for (int itrain=0; itrain<train_input.size(); itrain++) {

			vector<double> &inputi = train_input[itrain];
			// update ohs
			for (int i=0; i<ohs.size(); i++) {
				double sum = w0h[i];
				for (int j=0; j<inputi.size(); j++)
					sum += inputi[j]*w12[j][i];
				double o = 1.0/(1.0 + exp(-sum));
				ohs[i] = o;
			}

			// update oks
			for (int i=0; i<oks.size(); i++) {
				double sum = w0k[i];
				for (int j=0; j<ohs.size(); j++)
					sum += ohs[j]*w23[j][i];
				double o = 1.0/(1.0 + exp(-sum));
				oks[i] = o;
			}

			// get error
			vector<double> &outputi = train_output[itrain];
			double err = 0.0;
			for (int i=0; i<oks.size(); i++)
				err += sqr(oks[i]-outputi[i]);
			tot_err += err;

			for (int i=0; i<oks.size(); i++)
				o_errs[i] += sqr(oks[i]-outputi[i]);

			// output h_value
			if (itrain == 2) {
				for (int i=0; i<ohs.size(); i++)
					fprintf(f_h_value, "%.2f\t", ohs[i]);
				fprintf(f_h_value, "\n");
			}


			// update dks
			for (int i=0; i<dks.size(); i++)
				dks[i] = oks[i]*(1-oks[i])*(outputi[i]-oks[i]);

			// update dhs
			for (int i=0; i<dhs.size(); i++) {
				double wds = 0.0;
				for (int j=0; j<dks.size(); j++)
					wds += w23[i][j]*dks[j];
				dhs[i] = ohs[i]*(1-ohs[i])*wds;
			}

			// update w12
			for (int i=0; i<w12.size(); i++) {
				for (int j=0; j<w12[i].size(); j++)
					w12[i][j] += eta*dhs[j]*inputi[i];
			}
			for (int i=0; i<w0h.size(); i++)
				w0h[i] += eta*dhs[i];

			// update w23
			for (int i=0; i<w23.size(); i++) {
				for (int j=0; j<w23[i].size(); j++)
					w23[i][j] += eta*dks[j]*ohs[i];
			}
			for (int i=0; i<w0k.size(); i++)
				w0k[i] += eta*dks[i];

			//for (int i=0; i<oks.size(); i++)
			//	printf("%.1f ", oks[i]);
			//printf("\n");

			if (iter%100 == 0) {
				for (int i=0; i<ohs.size(); i++)
					printf("%.1f ", ohs[i]);
				printf("\n");
			}
		}

		// output output_err
		for (int i=0; i<oks.size(); i++) {
			fprintf(f_output_err, "%.2f\t", o_errs[i]);
			o_errs[i] = 0.0;
		}
		fprintf(f_output_err, "\n");

		// output w12[i][0]
		for (int i=0; i<w12.size(); i++)
			fprintf(f_w12, "%.2f\t", w12[i][0]);
		fprintf(f_w12, "\n");
		
		printf("iter %d tot_err %lf\n", iter, tot_err);
		eta *= scale;
		
	}

	fclose(f_output_err);
	fclose(f_h_value);
	fclose(f_w12);
}

int main(void)
{
	init_trainset();
	init_net();
	train();
	return 0;
}


