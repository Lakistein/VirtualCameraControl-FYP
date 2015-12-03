#include "ParticleSwarmOptimization.h"

ParticleSwarmOptimization::ParticleSwarmOptimization(const DzVec3 dirPOI, const DzVec3 p, const DzVec3 min, const DzVec3 max, const DzVec3 points[4], std::list<DzBox3> nodes)
{
	_gBest = 1000;
	_gBestAngle = 1000;
	_gBestPos = DzVec3();
	_nodes = nodes;
	_point = p;
	_dirPOI = dirPOI;
	_min = min;
	_max = max;
	_numOfParticles = 100;

	c0 = 0.8;
	c1 = 1.1;
	c2 = 1.5;
	for (int i = 0; i < 4; i++)
	{
		_points[i] = points[i];
	}
	// Random generator initializator
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_real<> x(min.m_x, max.m_x);
	std::uniform_real<> y(min.m_y, max.m_y);
	std::uniform_real<> z(min.m_z, max.m_z);
	std::uniform_real<> rnd(-10, 10);

	// Initialize particles
	for (int i = 0; i < _numOfParticles; i++)
	{
		DzVec3 vec = DzVec3();
		vec.m_x = x(gen);
		vec.m_y = y(gen);
		vec.m_z = z(gen);

		int err = Error(&vec, points, nodes);
		float angle = GetAngle(&dirPOI, &DzVec3(vec - p));
		DzVec3 vel = DzVec3(rnd(gen), rnd(gen), rnd(gen));

		if ((err == _gBest && angle < _gBestAngle) || err < _gBest)
		{
			_gBest = err;
			_gBestAngle = angle;
			_gBestPos = vec;
		}
		Particle p = Particle(vec, vel, err, angle);
		_particles[i] = p;
		this->vecPoints[i] = vec;
	}

	//Run(points);
}

ParticleSwarmOptimization::~ParticleSwarmOptimization()
{
}

int ParticleSwarmOptimization::Error(const DzVec3 * origin, const DzVec3 * points, std::list<DzBox3> nodes)
{
	int best = 0;
	bool intersect[4] = { false };
	DzLine3 line = DzLine3(*origin, *origin);
	std::list<DzBox3> ::iterator node;
	for (node = nodes.begin(); node != nodes.end(); ++node)
	{
		for (int i = 0; i < 4; i++)
		{
			if (intersect[i]) continue;

			line.setEnd(points[i]);
			if (node->testIntersection(line, 0))
			{
				intersect[i] = true;
				best++;
				if (best == 4)
					return 4;
			}
		}
	}
	return best;
}

DzVec3 ParticleSwarmOptimization::GetBestPoint()
{
	return _gBestPos;
}

DzVec3* ParticleSwarmOptimization::Run()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real<> rnd(0.1, 1);
	Particle *p;
	for (int j = 0; j < 100; j++)
	{
		for (int i = 0; i < _numOfParticles; i++)
		{
			p = &_particles[i];
			DzVec3 vel = c0 * p->velocity.normalized()
				+ (c1 * rnd(gen) * (p->lBestPos - p->currPos).normalized())
				+ (c2 * rnd(gen) * (_gBestPos - p->currPos).normalized());

			p->velocity = vel;
			p->currPos = p->currPos + p->velocity;

			if (p->currPos.m_x > _max.m_x) p->currPos.m_x = _max.m_x;
			else if (p->currPos.m_x < _min.m_x) p->currPos.m_x = _min.m_x;

			if (p->currPos.m_y > _max.m_y) p->currPos.m_y = _max.m_y;
			else if (p->currPos.m_y < _min.m_y) p->currPos.m_y = _min.m_y;

			if (p->currPos.m_z > _max.m_z) p->currPos.m_z = _max.m_z;
			else if (p->currPos.m_z < _min.m_z) p->currPos.m_z = _min.m_z;

			vecPoints[i] = p->currPos;
			int err = Error(&p->currPos, _points, _nodes);

			float angle = _dirPOI.getAngleTo(p->currPos - *_point) * 180.0 / 3.14159265358979323846;
			float angle2 = GetAngle(&_dirPOI, &DzVec3(p->currPos - *_point));

			// GLOBAL

			if ((err == _gBest && angle < _gBestAngle) || err < _gBest)
			{
				_gBest = p->localBest = err;
				_gBestAngle = p->bestAngle = angle;
				_gBestPos = p->lBestPos = p->currPos;
				continue;
			}

			// LOCAL

			if ((err == p->localBest && angle < p->bestAngle) || err < p->localBest)
			{
				p->localBest = err;
				p->bestAngle = angle;
				p->lBestPos = p->currPos;
				continue;
			}
		}
	}

	return vecPoints;
}

DzVec3 * ParticleSwarmOptimization::GetDirectionVector(const DzVec3 * a, const DzVec3 * b)
{
	return nullptr;
}

float ParticleSwarmOptimization::GetAngle(const DzVec3 * A, const DzVec3 * B)
{
	return acos(A->dot(*B) / (A->length() * B->length())) * 180.0 / 3.14159265358979323846;
}

//float ParticleSwarmOptimization::GetAngle(const DzVec3 * A, const DzVec3 * B)
//{
//	return acos(A->dot(*B)) * 180.0 / 3.14159265358979323846;
//}

void ParticleSwarmOptimization::SetNumberOfParticles(int num)
{
}

void ParticleSwarmOptimization::SetDirectionVectorOfPOI(const DzVec3 * vec)
{
}

void ParticleSwarmOptimization::SetNumberOfPoints(int num)
{
}

void ParticleSwarmOptimization::SetCenterPoint(const DzVec3 * point)
{
}
#include <ctime>

DzVec3* ParticleSwarmOptimization::Run2()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real<> rnd(0.1, 1);
	Particle *p;
	for (int i = 0; i < _numOfParticles; i++)
	{
		p = &_particles[i];
		DzVec3 vel = c0 * p->velocity.normalized() 
			+ (c1 * rnd(gen) * (p->lBestPos - p->currPos).normalized()) 
			+ (c2 * rnd(gen) * (_gBestPos - p->currPos).normalized());

		p->velocity = vel;
		p->currPos = p->currPos + p->velocity;

		if (p->currPos.m_x > _max.m_x) p->currPos.m_x = _max.m_x;
		else if (p->currPos.m_x < _min.m_x) p->currPos.m_x = _min.m_x;

		if (p->currPos.m_y > _max.m_y) p->currPos.m_y = _max.m_y;
		else if (p->currPos.m_y < _min.m_y) p->currPos.m_y = _min.m_y;

		if (p->currPos.m_z > _max.m_z) p->currPos.m_z = _max.m_z;
		else if (p->currPos.m_z < _min.m_z) p->currPos.m_z = _min.m_z;

		vecPoints[i] = p->currPos;
		int err = Error(&p->currPos, _points, _nodes);

		float angle = _dirPOI.getAngleTo(p->currPos - *_point) * 180.0 / 3.14159265358979323846;
		float angle2 = GetAngle(&_dirPOI, &DzVec3(p->currPos - *_point));

		// GLOBAL

		if ((err == _gBest && angle < _gBestAngle) || err < _gBest)
		{
			_gBest = p->localBest = err;
			_gBestAngle = p->bestAngle = angle;
			_gBestPos = p->lBestPos = p->currPos;
			continue;
		}

		// LOCAL

		if ((err == p->localBest && angle < p->bestAngle) || err < p->localBest)
		{
			p->localBest = err;
			p->bestAngle = angle;
			p->lBestPos = p->currPos;
			continue;
		}
	}
	return vecPoints;
}
