#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#define GA_RESOURCE_DIR "C:/Users/jan20/jan/Resources/Image/"

class BackgroundImage : public Util::GameObject {

public:
    BackgroundImage() : GameObject(
            std::make_unique<Util::Image>(GA_RESOURCE_DIR"Background/phase0.png"), 10) {
    }

    void NextPhase(const int phase) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(ImagePath(phase));
    }

private:
    inline std::string ImagePath(const int phase) {
        return GA_RESOURCE_DIR"Background/phase" + std::to_string(phase) + ".png";
    }
};

#endif //BACKGROUND_IMAGE_HPP
