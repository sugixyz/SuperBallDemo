#include"ObjectLoader.h"
#include"../Engine/CsvReader.h"

enum ObjectColumData : int
{
	MODEL_NAME,
	POSITION_X,
	POSITION_Y,
	POSITION_Z,
	ROTATE_X,
	ROTATE_Y,
	ROTATE_Z,
	SCALE_X,
	SCALE_Y,
	SCALE_Z,
	IS_STATIC,
	COLUM_SIZE
};

ObjectLoader::ObjectLoader()
{
}

ObjectLoader::~ObjectLoader()
{
}

std::vector<ObjectData> ObjectLoader::Load(std::string filename)
{
	CsvReader csv;
	csv.Load(filename);
	int width = csv.GetWidth();
	int height = csv.GetHeight();

	std::vector<ObjectData> list;
	list.clear();
	//もしCSVの幅がデータの要素数と合っていなければ抜ける
	if (width != COLUM_SIZE)return list;

	for (int y = 0;y < height; y++)
	{
		ObjectData data;
		data.modelName = csv.GetString(MODEL_NAME, y);
		data.position.x = csv.GetValue(POSITION_X, y);
		data.position.y = csv.GetValue(POSITION_Y, y);
		data.position.z = csv.GetValue(POSITION_Z, y);
		data.rotate.x = csv.GetValue(ROTATE_X, y);
		data.rotate.y = csv.GetValue(ROTATE_Y, y);
		data.rotate.z = csv.GetValue(ROTATE_Z, y);
		data.scale.x = csv.GetValue(SCALE_X, y);
		data.scale.y = csv.GetValue(SCALE_Y, y);
		data.scale.z = csv.GetValue(SCALE_Z, y);
		data.isStatic = static_cast<bool>(csv.GetValue(IS_STATIC, y));

		list.push_back(data);
	}

	return list;
}
