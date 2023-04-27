#pragma once

class Material {
public:
	
	void setShaderValue(float* Ka, float* Kd, float* Ks,float shiness=15) {
		setValue(&ka, Ka);
		setValue(&kd, Kd);
		setValue(&ks, Ks);
		this->shiness = shiness;
	}

	Material() {
		float* defaultKaValue = new float[3] {0.388, 0.388, 0.388};
		float* defaultKdValue = new float[3]{0.839, 0.839, 0.839};
		float* defaultKsValue = new float[3] {1,1,1};
		setShaderValue(defaultKaValue, defaultKdValue, defaultKsValue);
	}

	Material(float* Ka, float* Kd, float* Ks) {
		setShaderValue(Ka,Kd,Ks);
	}

	float* getKa() {
		return new float[3] {ka.r, ka.g, ka.b};
	}
	float* getKd() {
		return new float[3] {kd.r, kd.g, kd.b};
	}
	float* getKs() {
		return new float[3] {ks.r, ks.g, ks.b};
	}

	float getShiness() {return shiness;}

private:
	typedef struct {
		float r;
		float g;
		float b;
	}RGB;

	RGB ka;
	RGB kd;
	RGB ks;

	float shiness;

	void setValue(RGB* origin, float* value) {
		origin->r = value[0];
		origin->g = value[1];
		origin->b = value[2];
		//std::cout <<"Color:"<< origin.r << "," << origin.g << "," << origin.b << std::endl;
	}

};