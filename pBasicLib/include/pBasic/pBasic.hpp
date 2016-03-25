// This file is part of pBasic, a basic collection of concurrent
// algorithms and container.
//
// Copyright (C) 2016 Sascha Raesch <sascha.raesch@gmail.com>
//
// It is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution

#include <cstdint>

namespace pBasic
{
    /**
     * Details about the version of the pBasic distribution
     */
    namespace Version
    {
        static const std::uint32_t      major = 0;
        static const std::uint32_t      minor = 1;
        static const std::uint32_t      patch = 0;

        static const char               *versionString = "0.1.0";
    }

} // end of namespace pBasic
