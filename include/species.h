#ifndef  PSPM_PSPM_SPECIES_H_
#define  PSPM_PSPM_SPECIES_H_

#include <vector>
#include <list>
#include <string>

#include "iterator_set.h"

// forward declaration of Solver so Species can befriend it
template <class Model, class Environment>
class Solver;

template <class Model>
class Species{

	// All kinds of Solvers should be friends of Species
	template<class,class>
	friend class Solver;

	private: // private members
	int start_index;
	int J;	
	
	std::vector<std::string> varnames;			// state has internal variables (x, u) and possibly extra variables 
	std::vector<std::string> varnames_extra;		//   +-- which will be created in the state in this order (for each species)
	std::vector<int> strides;				// defines how the variables are packed into the state vector
	std::vector<int> offsets;				// 
	
	double birth_flux_in;

	std::list<double> birth_flux_out_history;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	
	// These are only used by FMU solver. 
	// Others derive them from the state. 
	// Kept private so users dont accidently access them for other solvers
	std::vector <double> X;	
	std::vector <double> x;
	std::vector <double> h;
	std::vector <double> schedule; // used only by CM/EBT

	public:
	double u0_save;

	public:
	//debug only
	bool bfin_is_u0in = false;
	
	private: // private functions
	int addVar(std::string name, int stride, int offset);
	void clearVars();

	public: // public members
	double xb, xm;  // FIXME. Dangerous because xm is never updated
	bool is_resident;
	
	Model * mod = nullptr;

	public: // public functions
	IteratorSet<std::vector<double>::iterator> get_iterators(std::vector<double> &v);
	double get_maxSize(std::vector<double>::iterator state_begin);
	std::vector<std::string> get_varnames();
	int xsize();
	int size();

	void set_model(Model *M);
	void set_inputBirthFlux(double b);
	double set_iStateVariables(std::vector<std::string> names);

	void set_bfin_is_u0in(bool flag);

	void print(std::vector<double> &sv, std::vector<double> &rv);
};

#include "../src/species.tpp"

#endif
