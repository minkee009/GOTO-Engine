#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <IRenderImage.h>

/// <summary>
/// 표시할 애니메이션의 정보 컨테이너입니다. 
/// 생성시 할당한 이미지들은 AnimaitonClip이 직접 관리합니다. 
/// 절대 관리하고 있는 이미지 배열을 인자로 넘겨 생성하지 마십시오
/// </summary>
class AnimationClip
{
public:
	AnimationClip(const std::vector<const GOTOEngine::IRenderImage*>& images, float frameRate, bool isLoop)
		: m_images(images), m_length(images.size()), m_frameRate(frameRate), m_isLoop(isLoop) {
	}
	~AnimationClip()
	{
		for (auto& img : m_images)
			delete img;
	}
	//static std::shared_ptr<AnimationClip> CreateFromFileStream(std::wstring filePath) { return; }
	static std::shared_ptr<AnimationClip> CreateShared(std::initializer_list<GOTOEngine::IRenderImage*> rawImages, float frameRate, bool isLoop)
	{
		std::vector<const GOTOEngine::IRenderImage*> images;
		for (GOTOEngine::IRenderImage* img : rawImages)
		{
			images.push_back(img);
		}
		return std::make_shared<AnimationClip>(images, frameRate, isLoop);
	}

	static AnimationClip* CreateRaw(std::initializer_list<GOTOEngine::IRenderImage*> rawImages, float frameRate, bool isLoop)
	{
		std::vector<const GOTOEngine::IRenderImage*> images;
		for (GOTOEngine::IRenderImage* img : rawImages)
		{
			images.push_back(img);
		}
		return new AnimationClip(images, frameRate, isLoop);
	}

	static AnimationClip* CreateRaw(std::vector<GOTOEngine::IRenderImage*> rawImages, float frameRate, bool isLoop)
	{
		std::vector<const GOTOEngine::IRenderImage*> images;
		for (GOTOEngine::IRenderImage* img : rawImages)
		{
			images.push_back(img);
		}
		return new AnimationClip(images, frameRate, isLoop);
	}

	const std::vector<const GOTOEngine::IRenderImage*>& GetKeyFrameImages() const { return m_images; }
	const size_t GetLength() const { return m_length; }
	const float GetFrameRate() const { return m_frameRate; }
	const bool IsLoop() const { return m_isLoop; }
private:
	const std::vector<const GOTOEngine::IRenderImage*> m_images;
	size_t m_length;
	float m_frameRate;
	bool m_isLoop;
};

class AnimationState
{
public:
	AnimationState(const std::wstring& name, const AnimationClip& clip) : m_name(name), m_clip(&clip) {}
	void ResetState();
	void UpdateFrame(float deltaTime);
	const GOTOEngine::IRenderImage& GetCurrentKeyFrameImage() const { return *m_currentImage; }
	bool IsLastFrame() const { return m_clip->IsLoop() ? (m_frameIdx == 0 && m_isCycleComplete) : (m_frameIdx == m_clip->GetLength() - 1) && m_isCycleComplete; }
	const std::wstring& GetName() { return m_name; }
private:
	const std::wstring m_name;
	const AnimationClip* m_clip;
	const GOTOEngine::IRenderImage* m_currentImage = nullptr;
	size_t m_frameIdx = 0;
	float m_frameTime = 0.0f;
	bool m_isCycleComplete = false;
};

class AnimationController
{
public:
	~AnimationController();
	void AddAnimationState(const std::wstring& name, const AnimationClip& clip);
	void Play(const std::wstring& name);
	void Update(float deltaTime);
	AnimationState& CurrentState() { return *m_currentState; }
	bool IsPlaying(const std::wstring& name);
private:
	std::unordered_map<std::wstring, AnimationState*> m_animationStates;
	AnimationState* m_currentState = nullptr;
	bool m_flipDraw = false;
};