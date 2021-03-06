#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "solver.h"
using namespace std;

#include "test_model_2_ms.h"

int main(){

	TestModel M;
	Environment E;

	Solver<TestModel,Environment> S(SOLVER_EBT);
	S.use_log_densities = false;
	//S.control.cm_grad_dx = 0.001;
	S.addSpecies(25, 0, 1, false, &M, {}, 2);
	S.resetState();
	S.initialize();
	S.setEnvironment(&E);
	S.get_species(0)->set_bfin_is_u0in(true);	// say that input_birth_flux is u0
	S.print();
	//for (auto s : S.state) cout << s << " "; cout << endl;

	E.computeEnv(0, S.state, &S);
	cout << "E = " << E.evalEnv(0,0) << endl;	
	
	S.calcRates_EBT(1, S.state, S.rates);  // dummy rates calc rates(X=X0, U=U0, t=1, E=E(U0))
	
	vector <double> rates_exp = {
		  0.0000000000,  0.2476464257,  0.2468536400,  0.2452680687,  0.2428897117,  0.2397185690,
		  0.2357546406,  0.2309979265,  0.2254484268,  0.2191061414,  0.2119710703,  0.2040432135,
		  0.1953225711,  0.1858091429,  0.1755029292,  0.1644039297,  0.1525121445,  0.1398275737,
		  0.1263502172,  0.1120800750,  0.0970171472,  0.0811614336,  0.0645129344,  0.0470716495,
		  0.0288375790,  0.0098107227,  0.4965383762, -0.1107483437, -0.0910785784, -0.0751940748,
		 -0.0622860653, -0.0517365749, -0.0430692323, -0.0359138025, -0.0299803350, -0.0250401308,
		 -0.0209115967, -0.0174496398, -0.0145376493, -0.0120813876, -0.0100043021, -0.0082439015,
		 -0.0067489372, -0.0054771977, -0.0043937710, -0.0034696703, -0.0026807389, -0.0020067756,
		 -0.0014308302, -0.0009386365, -0.0005181508, -0.0001591773
	    };
	
	vector <double> rates_exp2 = {   // exactly as per R
		  0.0000000000,  0.2476464257,  0.2468536400,  0.2452680687,  0.2428897117,  0.2397185690,
	      0.2357546406,  0.2309979265,  0.2254484268,  0.2191061414,  0.2119710703,  0.2040432135,
		  0.1953225711,  0.1858091429,  0.1755029292,  0.1644039297,  0.1525121445,  0.1398275737,
		  0.1263502172,  0.1120800750,  0.0970171472,  0.0811614336,  0.0645129344,  0.0470716495,
		  0.0288375790,  0.0098107227,  0.4954910479, -0.1107483437, -0.0910785784, -0.0751940748,
		 -0.0622860653, -0.0517365749, -0.0430692323, -0.0359138025, -0.0299803350, -0.0250401308,
		 -0.0209115967, -0.0174496398, -0.0145376493, -0.0120813876, -0.0100043021, -0.0082439015,
		 -0.0067489372, -0.0054771977, -0.0043937710, -0.0034696703, -0.0026807389, -0.0020067756,
		 -0.0014308302, -0.0009386365, -0.0005181508, -0.0001591773
		};

	
	for (int i=0; i< S.rates.size(); ++i){
		//cout << S.rates[i] << " " << rates_exp[i] << " " << rates_exp2[i] << endl;
		if ( fabs(S.rates[i] - rates_exp2[i]) > 1e-6) return 1;
	}
	
	ofstream fout("ebt_testmodel.txt");

	for (double t=0.05; t <= 8; t=t+0.05) {
		S.step_to(t);
		fout << S.current_time << "\t" << S.u0_out()[0] << "\t";
	
		//vector<double> v = S.cohortsToDensity_EBT(x);
		
		//cout << S.current_time << " " << S.get_species(0)->xsize() << " " << S.u0_out()[0] << endl;
		for (auto y : S.state) fout << y << "\t";
		fout << endl;
	}

	fout.close();
	
	cout << S.u0_out()[0] << endl;
	if (abs(S.u0_out()[0]-1.436407) < 2e-5) return 0;
	else return 1;
}

