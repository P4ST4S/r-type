/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TextSystem
*/

#include "TextSystem.hpp"

TextSystem::TextSystem(std::shared_ptr<RGraphic::IGraphic> lib) : _lib(lib)
{
}

TextSystem::~TextSystem()
{
}

void TextSystem::operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<TextComponent> &TextArray)
{
    Zipper<SparseArray<PositionComponent>, SparseArray<TextComponent>> zipper(positionArray, TextArray);

    for (auto it = zipper.begin(); it != zipper.end(); it++) {
        TextComponent &Text = std::get<TextComponent&>(*it);
        Vector2D<double> position = std::get<PositionComponent&>(*it).getPosition();
        _lib->drawText({position._x, position._y}, Text.getText().c_str(), Text.getFontSize(), Text.getOutlineSize(), Text.getColor(), Text.getOutlineColor());
    }
}
