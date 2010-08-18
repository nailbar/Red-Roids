#ifndef RR_VECTOR_H
#define RR_VECTOR_H 1

class RR_vector{
  public:
    double x,y;
    
    //Constructors
    RR_vector(){
      x=0;
      y=0;
    }
    RR_vector(double newx,double newy){
      x=newx;
      y=newy;
    }
};

#endif //RR_VECTOR_H
