/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TextureComponent
*/

#include "TextureComponent.hpp"

TextureComponent::TextureComponent() : _textures(), _frame(0), _nframes(0), _frameRate(10), _scale(1), _current(-1), _default(-1)
{
}

TextureComponent::~TextureComponent()
{
}

void TextureComponent::setDefaultTexture(int id)
{
    _default = id;
}

void TextureComponent::addTexture(int id, RGraphic::RTexture texture, RGraphic::RVect2_t dimension, uint16_t nframes, double scale, uint8_t frameRate)
{
    if (id == -1)
        return;
    if (_default == -1){
        _default = id;
        _current = id;
        _texture = texture;
        _frameRate = frameRate;
        _scale = scale;
        _nframes = nframes;
        _sourceRect.width = dimension.x / nframes;
        _sourceRect.height = dimension.y;
        computeSourcePosition();
    }
    _textures[id] = {texture, dimension, frameRate, nframes, scale};
}

void TextureComponent::playTexture(int id)
{
    TextureComponentData data;
    
    if (_current == id)
        return;
    auto found = _textures.find(id);
    if (found == _textures.end())
        return;
    data = found->second;
    _current = id;
    _frame = 0;
    _scale = data.scale;
    _texture = data.texture;
    _nframes = data.nframes;
    _sourceRect.width = data.dimension.x / data.nframes;
    _sourceRect.height = data.dimension.y;
    computeSourcePosition();
    _gap = std::chrono::high_resolution_clock::now();
    _prevGap = std::chrono::high_resolution_clock::now();
    _duration = this->_gap - this->_prevGap;
    _realDuration = this->_gap - this->_prevGap;
}

void TextureComponent::nextFrame()
{
    if (_default == -1)
        return;
    if (_nframes > 1){
        std::chrono::nanoseconds nsInterval(static_cast<size_t>(1e9 / _frameRate));
        this->_gap = std::chrono::high_resolution_clock::now();
        this->_duration = this->_gap - this->_prevGap;
        if (nsInterval < this->_duration){
            if (_default != _current && _frame + _duration / nsInterval >= _nframes)
                playTexture(_default);
            else {
                _frame = (_frame + _duration / nsInterval) % _nframes;
                computeSourcePosition();
                this->_prevGap = this->_gap;
            }
        }
    }
}


bool TextureComponent::hadTexture() const
{
    return (_current != -1);
}

RGraphic::RTexture TextureComponent::getTexture() const
{
    return (_texture);
}

RGraphic::RRectangle_t TextureComponent::getSourceRect() const
{
    return (_sourceRect);
}

double TextureComponent::getScale() const
{
    return (_scale);
}

uint16_t TextureComponent::getNFrames() const {
    return (_nframes);
}

uint16_t TextureComponent::getCurrentFrame() const {
    return (_frame);
}

int TextureComponent::getCurrentId() const
{
    return (_current);
}

void TextureComponent::computeSourcePosition()
{
    _sourceRect.x = _frame * _sourceRect.width;
    _sourceRect.y = 0;
}

void destroyTexture(Registry &registry, Entity const &entity)
{
    SparseArray<TextureComponent> &textureArray = registry.getComponents<TextureComponent>();

    if (textureArray.size() > entity)
        textureArray[entity].reset();
}
