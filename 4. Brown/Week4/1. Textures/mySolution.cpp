#include "Common.h"
#include "Textures.h"
#include<iostream>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`
class Shape : public IShape {
public:
    virtual bool PointInsideFigure(Point& p) const = 0;

    void SetPosition(Point p) override {
        position_ = p;
    }
    Point GetPosition() const override {
        return position_;
    }

    void SetSize(Size s) override {
        size_ = s;
    }
    Size GetSize() const override {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> txt) override {
        texture_ = txt;
    }
    ITexture* GetTexture() const override {
        return texture_.get();
    }
    std::shared_ptr<ITexture> GetTexturePtr() const {
        return texture_;
    }
    void Draw(Image& canvas) const override {
        Image texture;
        Size texture_size;
        Image default_txt(1, ".");
        Point start = GetPosition();

        if (GetTexture()) {
            texture = GetTexture()->GetImage();
            texture_size = { GetTexture()->GetSize().width, GetTexture()->GetSize().height };
        } else {
            texture = default_txt;
            texture_size = { 1, 1 };
        }

        for (int i = start.y; i - start.y < GetSize().height && i < canvas.size(); i++) {
            for (int j = start.x; j - start.x < GetSize().width && j < canvas[i].size(); j++) {
                Point p{ j - start.x, i - start.y };
                if (!PointInsideFigure(p)) continue;
                if (i - start.y < texture_size.height && j - start.x < texture_size.width) {
                    canvas[i][j] = texture[i - start.y][j - start.x];
                }
                else {
                    canvas[i][j] = '.';
                }
            }
        }
    }
private:
    std::shared_ptr<ITexture> texture_;
    Point position_ = { 0, 0 };
    Size size_ = { 0, 0 };
};

class Rectangle : public Shape {
public:
    Rectangle(Point p, Size s, std::shared_ptr<ITexture> t) {
        SetPosition(p);
        SetSize(s);
        SetTexture(t);
    }
    Rectangle() {
        SetPosition({ 0, 0 });
        SetSize({ 0, 0 });
        SetTexture(nullptr);
    }
    std::unique_ptr<IShape> Clone() const override {
        return std::make_unique<Rectangle>(GetPosition(), GetSize(), GetTexturePtr());
    }
 
    bool PointInsideFigure(Point& p) const override {
        if (p.x < GetSize().width && p.y < GetSize().height) {
            return true;
        } else {
            return false;
        }
    }
private:
};

class Ellipse : public Shape {
public:
    Ellipse(Point p, Size s, std::shared_ptr<ITexture> t) {
        SetPosition(p);
        SetSize(s);
        SetTexture(t);
    }
    Ellipse() {
        SetPosition({ 0, 0 });
        SetSize({ 0, 0 });
        SetTexture(nullptr);
    }
	std::unique_ptr<IShape> Clone() const override {
		return std::make_unique<Ellipse>(GetPosition(), GetSize(), GetTexturePtr());
	}
    bool PointInsideFigure(Point& p) const override {
        return IsPointInEllipse(p, GetSize());
    }
private:
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    if (shape_type == ShapeType::Rectangle) {
        return std::make_unique<Rectangle>();
    }
    return std::make_unique<Ellipse>();

}