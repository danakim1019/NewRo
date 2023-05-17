// https://github.com/danakim1019/NewRo
// The MIT License(MIT)
//
//Copyright (C) 2023 by danakim1019
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 

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
		float* defaultKaValue = new float[3] {0.141, 0.141, 0.141};
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