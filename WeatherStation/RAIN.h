
    // !!! This file is generated using emlearn !!!

    #include <eml_trees.h>
    

EmlTreesNode RAIN_nodes[5] = {
  { 0, 0.10000000149011612, 1, 2 },
  { -1, 0, -1, -1 },
  { 0, 0.9000000059604645, 3, 4 },
  { -1, 1, -1, -1 },
  { -1, 2, -1, -1 } 
};

int32_t RAIN_tree_roots[1] = { 0 };

EmlTrees RAIN = {
        5,
        RAIN_nodes,	  
        1,
        RAIN_tree_roots,
    };

static inline int32_t RAIN_predict_tree_0(const float *features, int32_t features_length) {
          if (features[0] < 0.10000000149011612) {
              return 0;
          } else {
              if (features[0] < 0.9000000059604645) {
                  return 1;
              } else {
                  return 2;
              }
          }
        }
        

int32_t RAIN_predict(const float *features, int32_t features_length) {

        int32_t votes[3] = {0,};
        int32_t _class = -1;

        _class = RAIN_predict_tree_0(features, features_length); votes[_class] += 1;
    
        int32_t most_voted_class = -1;
        int32_t most_voted_votes = 0;
        for (int32_t i=0; i<3; i++) {

            if (votes[i] > most_voted_votes) {
                most_voted_class = i;
                most_voted_votes = votes[i];
            }
        }
        return most_voted_class;
    }
    