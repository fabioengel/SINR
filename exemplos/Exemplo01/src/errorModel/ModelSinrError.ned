//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package exemplo01.src.errorModel;
import inet.physicallayer.wireless.common.base.packetlevel.ErrorModelBase;

module ModelSinrError extends ErrorModelBase
{
        parameters:
        double packetErrorRate = default(nan); // a probability in the range [0, 1] or NaN if not set
        double bitErrorRate = default(nan);    // a probability in the range [0, 1] or NaN if not set
        double symbolErrorRate = default(nan); // a probability in the range [0, 1] or NaN if not set
        @class(ModelSinrError);
}
