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
		//コメントだったら抜ける
		char str = csv.GetString(0, y).front();
		if (str == '#')continue;
		if (str == '/')continue;

		ObjectData data;
		data.modelName = csv.GetString(MODEL_NAME, y);
		data.position.x = csv.GetFloatValue(POSITION_X, y);
		data.position.y = csv.GetFloatValue(POSITION_Y, y);
		data.position.z = csv.GetFloatValue(POSITION_Z, y);
		data.rotate.x = csv.GetFloatValue(ROTATE_X, y);
		data.rotate.y = csv.GetFloatValue(ROTATE_Y, y);
		data.rotate.z = csv.GetFloatValue(ROTATE_Z, y);
		data.scale.x = csv.GetFloatValue(SCALE_X, y);
		data.scale.y = csv.GetFloatValue(SCALE_Y, y);
		data.scale.z = csv.GetFloatValue(SCALE_Z, y);
		std::string boolValue =  csv.GetString(IS_STATIC, y);
		data.isStatic = (boolValue == "true" || boolValue == "TRUE") ? true : false;

		list.push_back(data);
	}

	return list;
}
