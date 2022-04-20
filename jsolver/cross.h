#pragma once

#include <opencv2/opencv.hpp>

#include "field.h"
#include "hints.h"

namespace jsolver
{

class Cross
{
    using Pixel = uint8_t;

    cv::Mat     _image;
    Field       _source;
    HintsMap    _hints;
public:
    bool load(const std::string & path, size_t size)
    {
        _image = cv::imread(path, cv::IMREAD_COLOR);
        if (_image.empty())
        {
            std::cout << "Image File: " << path << " not found\n";
            return false;
        }

        cv::cvtColor(_image, _image, cv::COLOR_BGR2GRAY);
        cv::blur(_image, _image, cv::Size(5,5));
        cv::threshold(_image, _image, 127, 255, cv::THRESH_BINARY);

        cv::bitwise_not(_image, _image);
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(_image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        auto main_contour = std::max_element(contours.begin(), contours.end(), [](const auto & a, const auto & b)
        {
            return (cv::contourArea(a) <= cv::contourArea(b));
        });

        cv::Rect boundRect = cv::boundingRect(*main_contour);
        cv::bitwise_not(_image, _image);

        //cv::rectangle(_image, boundRect.tl(), boundRect.br(), cv::Scalar(0, 255, 0), 1);
        _image(boundRect).copyTo(_image);
        cv::resize(_image, _image, cv::Size(size, size));
        cv::threshold(_image, _image, 127, 255, cv::THRESH_BINARY);

        _source.init(_image.cols, _image.rows);

        _image.forEach<Pixel>([this](Pixel & p, const int * position)
        {
            CellState state = p < 127 ? CellState::FILLED : CellState::EMPTY;
            _source.set(position[1], position[0], state);
        });

        _source.print();
        _make_hints();

        return true;
    }

    std::tuple<uint32_t, uint32_t> size() const
    {
        return std::make_tuple(_source.width(), _source.height());
    }
    void write(const std::string & path) const
    {
        cv::imwrite(path, _image);
    }

    const HintsMap & hints() const
    {
        return _hints;
    }

private:
    void _make_hints()
    {
        uint32_t index = 0;
        for (size_t y = 0; y < size_t(_image.rows); ++y)
        {
            Hints hints;
            uint32_t count = 0;
            for (const auto & cell : _source.raw(y))
            {
                if (cell.get().is_filled())
                {
                    ++count;
                }
                else
                {
                    if (count > 0)
                    {
                        hints.emplace_back(++index, count);
                    }
                    count = 0;
                }
            }
            if (count > 0)
            {
                hints.emplace_back(++index, count);
            }
            _hints.left.push_back(hints);
        }

        for (size_t x = 0; x < size_t(_image.cols); ++x)
        {
            Hints hints;
            uint32_t count = 0;
            for (const auto & cell : _source.col(x))
            {
                if (cell.get().is_filled())
                {
                    ++count;
                }
                else
                {
                    if (count > 0)
                    {
                        hints.emplace_back(++index, count);
                    }
                    count = 0;
                }
            }
            if (count > 0)
            {
                hints.emplace_back(++index, count);
            }
            _hints.top.push_back(hints);
        }
    }
};

}