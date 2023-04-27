classdef ProcessingMethod
    enumeration
        Negative
        LR_Flip
        Darker
        Difference
        EdgeDetect
        HighPass
        None
    end

    methods(Static, Access = public)

        function result = MakeDarker(inputImg)
            result = inputImg - 64;
        end

        function result = MakeLRFlip(inputImg)
            result = fliplr(inputImg);
        end

        function result  = MakeNegative(inputImg)
            result = 255 - inputImg;
        end

        function result = DetectEdge(inputImg)
            result =  255*(repmat(uint8(edge(rgb2gray(inputImg), "sobel")), 1, 1, 3));
        end

        function result = GetHighPass(inputImg)
            result = imdilate(locallapfilt(inputImg, 0.5, 8), strel("square", 2));
        end

        function result = DoNotProcess(inputImg)
            result = inputImg;
        end

    end
end

