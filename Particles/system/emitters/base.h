#ifndef BASE_EMITTER
#define BASE_EMITTER

#include "..\..\..\common_h\math3d.h"
#include "..\..\icommon\iemitter.h"
#include "..\..\icommon\particle.h"
#include "..\..\icommon\types.h"
#include "..\datasource\datasource.h"
#include "..\particlesystem\particlesystem.h"
#include "..\datadesc\data_desc.h"

class DataGraph;
class BillBoardProcessor;


#define MAX_PARTICLES_COUNT 32


class BaseEmitter : public IEmitter
{




	struct structParticleType
	{
		bool Visible;										//Видим или нет
		ParticleType Type;							// Тип партикла
		float Remain;										// Сколько осталось незапущенных с прошлого кадра
		
		bool parity; //Четность для low-quality

		DWORD ActiveCount;							// Количество активных партиклов данного типа
		DWORD MaxParticlesCount;				// Максимальное кол-во партиклов этого типа
		DataGraph* EmissionRate;				// График задающий скорость испускания партиклов

		FieldList* pFields;


		structParticleType ()
		{
			ActiveCount = 0;
			Remain = 0.0f;
			EmissionRate = NULL;
			pFields = NULL;
			Type = UNKNOWN_PARTICLE;
		}

		
	};

	//array<structParticleType> ParticleTypes;

	dword ParticlesArraySize;
	structParticleType ParticlesArray[MAX_PARTICLES_COUNT];

protected:

	DataSource::EmitterDesc* pEmitter;


private:
	FieldList* pFields;
	bool IsAttachedFlag;


	Vector Position;
	EmitterType Type;
	float LifeTime;
	float ElapsedTime;
	DataGraph* EmissionDirX;
	DataGraph* EmissionDirY;
	DataGraph* EmissionDirZ;

	bool Looped;
	bool Stoped;
	bool Visible;

	DWORD Unique_GUID;

	Matrix matWorldTransform;
	bool OldMatrixNotInitialized;
	Matrix matWorldTransformOld;
	Matrix matWorldTransformNew;

	__forceinline void BlendMatrix (Matrix& result, const Matrix& mat1, const Matrix& mat2, float BlendK)
	{
		Quaternion qRot1(mat1);
		Quaternion qRot2(mat2);
		Vector vPos1 = mat1.pos;
		Vector vPos2 = mat2.pos;

		Quaternion qBlend;
		qBlend.SLerp(qRot1, qRot2, BlendK);

		Vector vBlend;
		vBlend.Lerp(vPos1, vPos2, BlendK);

		qBlend.GetMatrix(result);
		result.pos = vBlend;
	}




	void IncreaseTime (float DeltaTime);

protected:
  
	const char* szName;
	ParticleSystem* pMaster;

	Matrix matLastEmission;

  
	BillBoardProcessor* m_processor;

public:
 
	// Конструктор / деструктор
  BaseEmitter(ParticleSystem* pSystem, BillBoardProcessor* processor);
  virtual ~BaseEmitter();
  
	//Получить позицию для рождения новых партиклов  
	virtual Vector GetNewParticlePosition (float DeltaTime) = 0;
  

	//Родить новые партиклы 
	void BornParticles (float DeltaTime, float fTimeScale, float fScale, const Vector& additionalVelocity);
 	//Исполнить
  virtual void Execute (float DeltaTime, float fTimeScale, float fScale, const Vector& additionalVelocity);

	//Присоединиться к источнику данных
	virtual void AttachToDataSource (DataSource::EmitterDesc* pEmitter);



	virtual void CreateBillBoardParticle (FieldList &Fields);
	virtual void CreateModelParticle (FieldList &Fields);


	void GetEmissionDirection (Matrix &matWorld);


	virtual void SetGUID (DWORD GUID)
	{
		Unique_GUID = GUID;
	}

	virtual DWORD GetGUID ()
	{
		return Unique_GUID;
	}

	virtual void Restart ();


	virtual DWORD GetParticleCount ();
	virtual bool IsStoped ();

	virtual void SetTransform (const Matrix& matWorld);
	virtual void Teleport (const Matrix &matWorld);

	virtual const char* GetName ();

	//Если флаг в true емиттер не будет самостоятельно испускать партиклы
	//так, как он привязан
	virtual void SetAttachedFlag (bool Flag);
	virtual bool IsAttached ();

	virtual float GetTime ();
	virtual void SetTime (float Time);


	virtual DWORD GetParticleTypesCount ();
	virtual FieldList* GetParticleTypeDataByIndex (DWORD Index);
	virtual ParticleType GetParticleTypeByIndex  (DWORD Index);


	virtual FieldList* GetData ();


	virtual bool SetEnable (bool bVisible);
	virtual bool GetEnable ();

	//-1 если не нашли, иначе индекс
	virtual int GetParticleTypeIndex (FieldList* pFields);
	virtual bool SetParticleTypeEnable (bool bVisible, DWORD Index);
	virtual bool GetParticleTypeEnable (DWORD Index);


	virtual void Editor_UpdateCachedData ();

	virtual void SetName (const char* Name);


	virtual Matrix& GetTransform ();

	virtual void GetSystemTransform (Matrix& trans);


	//virtual void SetLocalMode (bool bForce);


};

#endif