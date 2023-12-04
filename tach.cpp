 #include "tach.h"
 
 //64 counts per rev
 
 Tach::Tach(PinName input, 
            int pulsesPerRev): input_(input){
    speed_       = 0.0;
    pulsesPerRev_ = pulsesPerRev;
    input_.mode(PullUp);
    input_.rise(callback(this, &Tach::update));
    //input_.fall(this, &Tach::update); callback(&Tach, &Tach::update)
    timer_.start();
    for(int i=0;i<NSAMP;i++){
        speedBuffer_[i]=0.0;
    }
}
int Tach::getCount(void){
    return count_;
}
float Tach::getSpeed(void) {

    float acc=0;
    for(int i=0;i<NSAMP;i++){
        acc+=speedBuffer_[i];
    }
    return acc/NSAMP;

}

void Tach::update(void) {
    float speed;
    static int i=0;
    count_++;
    speed =  1.0/(timer_.read()*pulsesPerRev_); // in Hz, i.e. rev/s
    speedBuffer_[i%NSAMP]=speed;
    i++;
    timer_.reset();
}