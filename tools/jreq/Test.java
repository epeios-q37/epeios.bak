/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

	This file is part of JREq.

	JREq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	JREq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with JREq. If not, see <http://www.gnu.org/licenses/>.
*/

public class Test {
	private static void displayCompilationTime() throws Exception {
		System.out.println("Bytecode build : " + new java.util.Date(new java.io.File(Test.class.getClassLoader()
				.getResource(Test.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()));
	}

	public static void main(String[] args) throws Exception {
		info.q37.jreq.JRE.Init();
		System.out.println(info.q37.jreq.JRE.wrapperInfo());
		displayCompilationTime();
	}
}
