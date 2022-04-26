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

package exemplo01.src.device;
import inet.mobility.contract.IMobility;
import inet.node.inet.INetworkNode;
import inet.node.inet.WirelessHost;
import inet.node.inet.AdhocHost;

module Host extends AdhocHost{
//module Host extends WirelessHost like INetworkNode{

    parameters:
        //@networkNode;
        @display("i=device/mobile");
        int numHosts = default(5);
        int myId @mutable = default(-1);

    submodules:
        hostConfig: HostConfig {}
}