#pragma once

template<class type>
class DataStorage
{
public:
	DataStorage(){}
	~DataStorage(){}

	// 各アセットの読み込み・取得関数
	std::shared_ptr<type> LoadData(std::string_view fileName)
	{
		std::shared_ptr<type> newData = std::make_shared<type>();

		if (!newData->Load(fileName.data()))
		{
			assert(0 && "ファイルが存在しません");
			return nullptr;
		}

		m_spDatas[fileName.data()] = newData;

		return newData;
	}

	// データの取得:リストにない場合新しくロードする
	std::shared_ptr<type> GetData(std::string_view fileName)
	{
		// リストに欲しいデータがあるか検索
		auto findData = m_spDatas.find(fileName.data());

		// データがあった場合
		if (findData != m_spDatas.end())
		{
			// データを共有
			return (*findData).second;
		}
		else
		{
			// 新たにデータをロードする
			return LoadData(fileName);
		}
	}

	// 保持しているデータの破棄
	void ClearData(bool force)
	{
		if (force)
		{
			// 強制的に全てのデータを消去
			m_spDatas.clear();

			return;
		}

		// アプリ上で使用されておらずStorageクラスが保持しているだけのデータを破棄
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