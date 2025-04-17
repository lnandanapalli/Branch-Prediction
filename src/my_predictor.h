// my_predictor.h
// This file contains a sample gshare_predictor class.
// It is a simple 32,768-entry gshare with a history length of 15.

class gshare_update : public branch_update {
public:
	unsigned int index;
};

class gshare_predictor : public branch_predictor {
public:
#define HISTORY_LENGTH	15
#define TABLE_BITS	15
	gshare_update u;
	branch_info bi;
	unsigned int history;
	unsigned char tab[1<<TABLE_BITS];

	gshare_predictor (void) : history(0) { 
		memset (tab, 0, sizeof (tab));
	}

	branch_update *predict (branch_info & b) {
		bi = b;
		if (b.br_flags & BR_CONDITIONAL) {
			u.index = 
				  (history << (TABLE_BITS - HISTORY_LENGTH)) 
				^ (b.address & ((1<<TABLE_BITS)-1));
			u.direction_prediction (tab[u.index] >> 1);
		} else {
			u.direction_prediction (true);
		}
		u.target_prediction (0);
		return &u;
	}

	void update (branch_update *u, bool taken, unsigned int target) {
		if (bi.br_flags & BR_CONDITIONAL) {
			unsigned char *c = &tab[((gshare_update*)u)->index];
			if (taken) {
				if (*c < 3) (*c)++;
			} else {
				if (*c > 0) (*c)--;
			}
			history <<= 1;
			history |= taken;
			history &= (1<<HISTORY_LENGTH)-1;
		}
	}
};

//
// Pentium M hybrid branch predictors
// This class implements a simple hybrid branch predictor based on the Pentium M branch outcome prediction units. 
// Instead of implementing the complete Pentium M branch outcome predictors, the class below implements a hybrid 
// predictor that combines a bimodal predictor and a global predictor. 
class pm_update : public branch_update {
public:
        unsigned int index;
		unsigned int g_index;
};

class pm_predictor : public branch_predictor {
public:
#define HISTORY_LENGTH	5
#define TABLE_BITS	5

    pm_update upd;
	branch_info binfo;
	
	unsigned char btab[1<<TABLE_BITS];
	unsigned char gtab[1<<TABLE_BITS];
	unsigned int hist;
	
	pm_predictor (void) : hist (0) {
		// Initialize gtab to zero
		for (int i = 0; i < sizeof(gtab) / sizeof(gtab[0]); i++) {
			gtab[i] = 0;
		}

		// Initialize btab to zero
		for (int i = 0; i < sizeof(btab) / sizeof(btab[0]); i++) {
			btab[i] = 0;
		}
	}

	branch_update *predict (branch_info & b) {
		binfo=b;

		if (BR_CONDITIONAL & b.br_flags) {

			unsigned int temp = 1<<TABLE_BITS;
			upd.index = b.address & (temp-1);
			unsigned int tb_hl = (TABLE_BITS - HISTORY_LENGTH);
			unsigned int g_index_temp = (hist << tb_hl);
			upd.g_index = (hist << tb_hl) ^ upd.index;
			unsigned int h = hist >> 1 << 1;
			
			if (h) {
				upd.direction_prediction(gtab[upd.g_index]>>1);
			}
			 else {
				upd.direction_prediction(btab[upd.index]>>1);
			}

		} else { 
			upd.direction_prediction(1);
		}

		upd.target_prediction(0);
		return &upd;
	}

    void update (branch_update *u, bool taken, unsigned int target) {
		if (BR_CONDITIONAL & binfo.br_flags) {
			unsigned char *b = &btab[((pm_update*)u)->index];
			unsigned char *g = &gtab[((pm_update*)u)->g_index];

			if (taken) 
			{
				if (*g < 3)
					(*g)++; 	
				if (*b < 3)
					(*b)++;			
			} else {
				if (*g > 0) 
					(*g)--; 
				if (*b > 0)
					(*b)--;
			}

			hist <<= 1;
			hist |= taken;
			hist &= (1<<HISTORY_LENGTH)-1;
		}
	}
};

//
// Complete Pentium M branch predictors for extra credit
// This class implements the complete Pentium M branch prediction units. 
// It implements both branch target prediction and branch outcome predicton. 
class cpm_update : public branch_update {
public:
        unsigned int index;
};

class cpm_predictor : public branch_predictor {
public:
        cpm_update u;

        cpm_predictor (void) {
        }

        branch_update *predict (branch_info & b) {
            u.direction_prediction (true);
            u.target_prediction (0);
            return &u;
        }

        void update (branch_update *u, bool taken, unsigned int target) {
        }

};


