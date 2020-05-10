#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->frame.clear();

	int number_of_satellite = 2;
	int number_of_log = 50;
	int base_radius = 30;

	for (int i = 0; i < 8; i++) {

		auto base_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		auto base_location = glm::vec3(
			ofMap(ofNoise(base_seed.x, (ofGetFrameNum() + number_of_log) * 0.005), 0, 1, -300, 300),
			ofMap(ofNoise(base_seed.y, (ofGetFrameNum() + number_of_log) * 0.005), 0, 1, -300, 300),
			ofMap(ofNoise(base_seed.z, (ofGetFrameNum() + number_of_log) * 0.005), 0, 1, -300, 300));

		for (int s = 0; s < number_of_satellite; s++) {

			float noise_seed_x = ofRandom(1000);
			float noise_seed_y = ofRandom(1000);
			auto location = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
			location = glm::normalize(location) * base_radius * 1.2;

			for (int k = 0; k < number_of_log; k++) {

				auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed_y, (ofGetFrameNum() + k) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(0, 1, 0));
				auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed_x, (ofGetFrameNum() + k) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(1, 0, 0));
				auto tmp_base_location = glm::vec3(
					ofMap(ofNoise(base_seed.x, (ofGetFrameNum() + k) * 0.005), 0, 1, -300, 300),
					ofMap(ofNoise(base_seed.y, (ofGetFrameNum() + k) * 0.005), 0, 1, -300, 300),
					ofMap(ofNoise(base_seed.z, (ofGetFrameNum() + k) * 0.005), 0, 1, -300, 300));
				auto loc = tmp_base_location + glm::vec4(location, 0) * rotation_y * rotation_x;
				this->frame.addVertex(loc);
				this->frame.addColor(ofColor(255, ofMap(k, 0, number_of_log, 25, 255)));

				int index = this->frame.getNumVertices() - 1;
				if (k > 0) {

					this->frame.addIndex(index); this->frame.addIndex(index - 1);
				}

				if (index > number_of_log && s > 0) {

					this->frame.addIndex(index); this->frame.addIndex(index - number_of_log);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int i = 0; i < this->frame.getNumVertices(); i++) {

		ofSetColor(this->frame.getColor(i));
		ofDrawSphere(this->frame.getVertex(i), 1.5);
	}

	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}