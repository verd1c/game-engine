#pragma once


#include<string>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>

class Audio {

private:

	
public:
	ALLEGRO_SAMPLE* sample;//current sample
	ALLEGRO_VOICE* Voice;
	ALLEGRO_MIXER* Mixer;
	
	std::vector< ALLEGRO_SAMPLE*> samples_vec;
	std::vector< ALLEGRO_SAMPLE_INSTANCE*> samples_instances_vec;


	ALLEGRO_SAMPLE_INSTANCE* sample_instance;
	static Audio singleton;
	ALLEGRO_SAMPLE_ID sampleID;
	void playSample(std::string, ALLEGRO_PLAYMODE mode);
	void stopSample();
	Audio();

	~Audio();

};



