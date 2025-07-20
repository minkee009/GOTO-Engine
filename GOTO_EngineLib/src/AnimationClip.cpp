#include "AnimationClip.h"
#include <filesystem>
#include <fstream>
#include <json.hpp>
#include <iostream>
#include "Texture2D.h"
#include "WStringHelper.h"


using json = nlohmann::json;

void GOTOEngine::AnimationClip::LoadFromFilePath(const std::wstring& filePath)
{
	std::ifstream inFile(filePath);
	if (inFile.is_open())
	{
		json jFromFile;
		inFile >> jFromFile;
		inFile.close();
		m_clipName = STR_TO_WSTR(jFromFile["clipName"]);
		m_texturePath = STR_TO_WSTR(jFromFile["texturePath"]);
		std::filesystem::path p(m_texturePath);

		//��������Ʈ ��Ʈ ��
		std::unordered_map<std::string, Sprite*> spriteSheet;
		auto spriteSheetPath = p.stem().wstring() + L"_sprites.json";
		spriteSheetPath = p.parent_path().wstring() + L"/" + spriteSheetPath;
		std::ifstream inSheetFile(spriteSheetPath);
		if (inSheetFile)
		{
			json jFromSheetFile;
			inSheetFile >> jFromSheetFile;
			inSheetFile.close();
			for (auto spriteInfo : jFromSheetFile["sprites"])
			{
				auto createdSprite = new Sprite();

				createdSprite->SetTexture(Load<Texture2D>(m_texturePath));
				createdSprite->SetRect(
					Rect{ spriteInfo["x"],spriteInfo["y"],spriteInfo["width"],spriteInfo["height"] });
				createdSprite->SetPivotX(spriteInfo["pivotX"]);
				createdSprite->SetPivotY(spriteInfo["pivotY"]);

				spriteSheet[spriteInfo["name"]] = createdSprite;
			}
		}

		m_isLoop = jFromFile["loop"];
		m_duration = jFromFile["duration"];

		//��������Ʈ ����
		for (auto keyframe : jFromFile["frames"])
		{
			if (spriteSheet.find(keyframe["sprite"]) == spriteSheet.end())
			{
				//��Ȯ�� ��������Ʈ�� ��� �����ߴ� �� �ϰ��ı�
				for (auto spr : spriteSheet)
				{
					DestroyImmediate(spr.second);
				}
				Dispose();
				break;
			}

			m_keyframes.emplace_back(new AnimationKeyframe{ spriteSheet[keyframe["sprite"]],keyframe["time"] });
			spriteSheet[keyframe["sprite"]]->IncreaseRefCount();
		}
	}
}

void GOTOEngine::AnimationClip::Dispose()
{
	for (auto frame : m_keyframes)
	{
		DestroyImmediate(frame);
	}
	m_keyframes.clear();
}

