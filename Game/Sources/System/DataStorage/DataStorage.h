#pragma once

template<class type>
class DataStorage
{
public:
	DataStorage(){}
	~DataStorage(){}

	// �e�A�Z�b�g�̓ǂݍ��݁E�擾�֐�
	std::shared_ptr<type> LoadData(std::string_view fileName)
	{
		std::shared_ptr<type> newData = std::make_shared<type>();

		if (!newData->Load(fileName.data()))
		{
			assert(0 && "�t�@�C�������݂��܂���");
			return nullptr;
		}

		m_spDatas[fileName.data()] = newData;

		return newData;
	}

	// �f�[�^�̎擾:���X�g�ɂȂ��ꍇ�V�������[�h����
	std::shared_ptr<type> GetData(std::string_view fileName)
	{
		// ���X�g�ɗ~�����f�[�^�����邩����
		auto findData = m_spDatas.find(fileName.data());

		// �f�[�^���������ꍇ
		if (findData != m_spDatas.end())
		{
			// �f�[�^�����L
			return (*findData).second;
		}
		else
		{
			// �V���Ƀf�[�^�����[�h����
			return LoadData(fileName);
		}
	}

	// �ێ����Ă���f�[�^�̔j��
	void ClearData(bool force)
	{
		if (force)
		{
			// �����I�ɑS�Ẵf�[�^������
			m_spDatas.clear();

			return;
		}

		// �A�v����Ŏg�p����Ă��炸Storage�N���X���ێ����Ă��邾���̃f�[�^��j��
		for (auto dataIter = m_spDatas.begin(); dataIter != m_spDatas.end();)
		{
			if (dataIter->second.use_count() < 2)
			{
				dataIter = m_spDatas.erase(dataIter);

				continue;
			}

			++dataIter;
		}
	}

private:
	std::unordered_map<std::string, std::shared_ptr<type>> m_spDatas;
};

class Assets
{
public:
	DataStorage<Texture> m_textures;
	DataStorage<ModelData> m_modelDatas;

	static Assets& GetInstance()
	{
		static Assets instance;
		return instance;
	}

	void ClearData(bool force)
	{
		m_textures.ClearData(force);
		m_modelDatas.ClearData(force);
	}

private:
	void Release()
	{
		ClearData(true);
	}

	Assets(){}
	~Assets() { Release(); }

};