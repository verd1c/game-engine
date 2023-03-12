#include "game/audio.h"



void Audio::playSample(std::string path,  ALLEGRO_PLAYMODE moder ) {
	//"config/sound.mp3"
	this->sample = al_load_sample(path.c_str());


	sample_instance = al_create_sample_instance(sample);


	samples_vec.push_back(sample);
	samples_instances_vec.push_back(sample_instance);



	al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
	al_set_sample_instance_playing(sample_instance, 1);



}

void Audio::stopSample() {

		//al_stop_sample(&sampleID);
}


Audio::~Audio() {
	printf("clearing audio\n");
	delete sample;
}


Audio::Audio() {
	//this->sample = NULL;
}