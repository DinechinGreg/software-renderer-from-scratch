#pragma once

namespace culling
{

enum class Type
{
    None,
    BackFace,
    FrontFace
};

static Type opposite(Type culling)
{
    switch (culling)
    {
    case Type::BackFace:
        return Type::FrontFace;
    case Type::FrontFace:
        return Type::BackFace;
    }
    return Type::None;
}

} // namespace culling
